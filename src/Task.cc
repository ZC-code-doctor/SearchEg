#include "Task.h"

#include <sw/redis++/redis++.h>
#include <fstream>

using namespace sw::redis;
using std::ifstream;

Task::~Task() {}

// 发送任务，接收、发送分离
SendTask::SendTask(factor &&cb)
    : _cb(std::move(cb))
{
}
void SendTask::producess()
{
    // 通过IO服务器对象维护的线程池进行数据的发送
    _cb();
    cout << "完成发送任务" << endl;
}

// 创建一个HTTP响应的任务包，包含发送数据，和连接
HttpPackge::HttpPackge(string msg, Tcpconnection_ptr con)
    : _msg(msg), _conn(con)
{
}
HttpTask::HttpTask(HttpPackge packge)
    : _packge(packge)
{
}
void HttpTask::producess()
{
    // 子线程处理业务
    std::ifstream file(_packge._msg);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << _packge._msg << std::endl;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();                 // 读取整个文件内容
    std::string htmlContent = buffer.str(); // 转换为std::string

    // 构建HTTP响应头
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << htmlContent.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n"; // 空行后开始正文
    response << htmlContent;
    // 发送HTTP响应
    std::string responseStr = response.str();

    // 子线程处理完业务，往EventLoop中的vector<factor>中添加一个数据
    _packge._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _packge._conn.get(), responseStr));
}

// 搜索网页任务
SearchPackge::SearchPackge(string keyWord, Engine_basic *engine, Tcpconnection_ptr con)
    : _keyWord(keyWord), _engine(engine), _conn(con)
{
}
SearchTask::SearchTask(SearchPackge packge)
    : _packge(packge)
{
}


//redis
/*
void SearchTask::producess()
{
    // 先向Redis服务器发送一个keyWord

    // 创建 Redis 对象，连接到 Redis 服务器
    auto redis = Redis("tcp://127.0.0.1:6379");

    // 获取 key 的值
    auto val = redis.get(_packge._keyWord);
    if (val)    
    {
        //如果key值存在则直接返回查询结果
        // 子线程处理完业务，往EventLoop中的vector<factor>中添加一个数据
        string mes = *val;
        _packge._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _packge._conn.get(),mes));
        cout << "redis 缓存返回" << endl;
    }
    else    
    {
        // 如果返回值为空，则调用引擎查询底层数据，并插入一个key-value

        // 获取json集合
        json jsonResponseArray = _packge._engine->SearchPage(_packge._keyWord);
        // 将 JSON 数组序列化为字符串
        std::string jsonResponse = jsonResponseArray.dump();
        // 构建 HTTP 响应报文
        std::string response;
        response += "HTTP/1.1 200 OK\r\n";                                             // 状态行
        response += "Content-Type: application/json\r\n";                              // 设置内容类型为 JSON
        response += "Content-Length: " + std::to_string(jsonResponse.size()) + "\r\n"; // 设置内容长度
        response += "Connection: keep-alive\r\n";                                      // 连接类型
        response += "\r\n";                                                            // 空行，分隔头部和主体
        response += jsonResponse;                                                      // 响应体

        // 子线程处理完业务，往EventLoop中的vector<factor>中添加一个数据
        _packge._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _packge._conn.get(), response));
        //并向redis插入一个键，关键词对应的HTTP响应报文
        redis.set(_packge._keyWord, response);
        cout << "key does not exist" << endl;
    }
}
*/
void SearchTask::producess()
{
    //获取json集合
    json jsonResponseArray = _packge._engine->SearchPage(_packge._keyWord);
    // 将 JSON 数组序列化为字符串
    std::string jsonResponse = jsonResponseArray.dump();
    // 构建 HTTP 响应报文
    std::string response;
    response += "HTTP/1.1 200 OK\r\n";  // 状态行
    response += "Content-Type: application/json\r\n";  // 设置内容类型为 JSON
    response += "Content-Length: " + std::to_string(jsonResponse.size()) + "\r\n";  // 设置内容长度
    response += "Connection: keep-alive\r\n";  // 连接类型
    response += "\r\n";  // 空行，分隔头部和主体
    response += jsonResponse;  // 响应体

    //子线程处理完业务，往EventLoop中的vector<factor>中添加一个数据
    _packge._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _packge._conn.get(), response));
}

// 推荐关键词任务
RecommendPackge::RecommendPackge(string keyWord, Engine_basic *engine, Tcpconnection_ptr con)
    : _keyWord(keyWord), _engine(engine), _conn(con)
{
}
RecommendTask::RecommendTask(RecommendPackge packge)
    : _packge(packge)
{
}
void RecommendTask::producess()
{
    // 获取 string 集合
    vector<string> recommend = _packge._engine->recommendWord(_packge._keyWord);
    // 构建 JSON 响应体
    json jsonResponseArray = json::array(); // 创建 JSON 数组
    for (const auto &elem : recommend)
    {
        jsonResponseArray.push_back(elem); // 将每个字符串放入 JSON 数组
    }
    // 将 JSON 数组序列化为字符串
    std::string jsonResponse = jsonResponseArray.dump();

    // 构建 HTTP 响应报文
    std::string response;
    response += "HTTP/1.1 200 OK\r\n";                                             // 状态行
    response += "Content-Type: application/json\r\n";                              // 设置内容类型为 JSON
    response += "Content-Length: " + std::to_string(jsonResponse.size()) + "\r\n"; // 设置内容长度
    response += "Connection: keep-alive\r\n";                                      // 连接类型
    response += "\r\n";                                                            // 空行，分隔头部和主体
    response += jsonResponse;                                                      // 响应体

    // 子线程处理完业务，往 EventLoop 中的 vector<factor> 中添加一个数据
    _packge._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _packge._conn.get(), response));
}