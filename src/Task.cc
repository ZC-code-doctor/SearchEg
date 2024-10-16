#include "Task.h"
#include <fstream>

using std::ifstream;

Task::~Task() {}

TaskA::TaskA(string str)
    : name(str) {}

void TaskA::producess()
{
    cout << "This is a TaksA:" << name << endl;
}
TaskA::~TaskA() {}

void TaskB::producess()
{
    cout << "This is a TaksB:" << number << endl;
}

TaskB::TaskB(int num)
    : number(num) {}

TaskB::~TaskB() {}

Packge::Packge(string &msg, Tcpconnection_ptr con)
    : _msg(msg), _conn(con)
{
}

TaskC::TaskC(Packge &pack)
    : _pack(pack)
{
}

void TaskC::producess()
{
    // 子线程处理业务
    _pack._msg = "This is TaskC handle:" + _pack._msg;
    // 子线程处理完业务，往EventLoop中的vector<factor>中添加一个数据
    _pack._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _pack._conn.get(), _pack._msg));
}

TaskD::TaskD(factor &&cb)
    : _cb(std::move(cb))
{
}
void TaskD::producess()
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
    buffer << file.rdbuf(); // 读取整个文件内容
    std::string htmlContent = buffer.str();    // 转换为std::string

    // 构建HTTP响应头
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << htmlContent.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";  // 空行后开始正文
    response << htmlContent;
    // 发送HTTP响应
    std::string responseStr = response.str();

    // 子线程处理完业务，往EventLoop中的vector<factor>中添加一个数据
    _packge._conn->addEventPending(std::bind(&Tcpconnection::sendMsg, _packge._conn.get(), responseStr));
}