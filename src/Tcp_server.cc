#include "Tcp_server.h"
#include "Tools.h"

Tcp_server::Tcp_server(const string &ip, unsigned short port, Engine *engine)
    : _acceptor(ip, port), _ioserver(), _acceptserver(_acceptor, _ioserver), _pool(3), _engine(engine)
{
}

Tcp_server::~Tcp_server()
{
}
/*
void Tcp_server::Start()
{
    _acceptor.ready();
    _pool.Start();

    setFunction();

    std::thread th1(std::bind(&IOserver::loop, &_ioserver));
    _acceptserver.Start();
    th1.join();
}
*/

void Tcp_server::Start()
{
    _acceptor.ready();
    _pool.Start();

    setFunction();

    // 启动 IOserver 的事件循环线程
    std::thread ioThread(std::bind(&IOserver::loop, &_ioserver));

    // 启动 AcceptServer 的处理线程，防止主线程被阻塞
    std::thread acceptThread(&AcceptServer::Start, &_acceptserver);

    // 主线程的定时任务
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        // 执行定时任务的代码
        std::cout << "定时任务执行" << std::endl;
        //定时更新缓存
        _engine->updatCache();

    }

    // 等待子线程完成
    ioThread.join();
    acceptThread.join();
}

void Tcp_server::Stop()
{
    _acceptserver.Stop();
    _ioserver.unloop();
    _pool.Stop();
}
void Tcp_server::setFunction()
{
    _ioserver.setCallbackClose(std::bind(&Tcp_server::onClose, this, std::placeholders::_1));
    _ioserver.setCallbackMessage(std::bind(&Tcp_server::onMessage, this, std::placeholders::_1));
    _ioserver.setCallbackNewConnect(std::bind(&Tcp_server::onNewConnection, this, std::placeholders::_1));
}

// 处理收到的消息
void Tcp_server::onMessage(const Tcpconnection_ptr &con)
{
    // 接收客户端的数据
    string msg = con->recvMsg();

    // TODO:服务器接收到消息后解析指令
    HttpRequest recv = parseHttpRequest(msg);

    // 1、HTTP GET任务则打包成发送原始网页
    // 2、HTTP POST任务则打包成获取网页任务
    // 3、HTTP PUT任务则打包成关键词推荐任务

    if (recv.method == "GET")
    {
        // 将收到数据和TCP对象打包
        HttpPackge pack("/home/lzc/SearchEg/data/search.html", con);
        // 将任务包发给子线程(con、msg)
        addTask(_pool, pack);
        std::cout << "请求方法" << recv.method << "," << "URL" << recv.url << "\n";
        std::cout << "返回一个网页" << "\n";
    }
    else if (recv.method == "POST")
    {
        //"/search?keyword=中文关键字"
        string keyWord = parseUrl(recv.url);
        SearchPackge pack(keyWord, _engine, con);
        addTask(_pool, pack);
        std::cout << "搜索任务" << "\n";
    }
    else if (recv.method == "PUT")
    {
        //"/search?keyword=中文关键字"
        string keyWord = parseUrl(recv.url);
        RecommendPackge pack(keyWord, _engine, con);
        addTask(_pool, pack);
        std::cout << "推荐任务" << "\n";
    }
}
void Tcp_server::onNewConnection(const Tcpconnection_ptr &con)
{
    cout << con->toString() << " has connected!!!" << endl;
}
void Tcp_server::onClose(const Tcpconnection_ptr &con)
{
    cout << con->toString() << " has closed!!!" << endl;
}
