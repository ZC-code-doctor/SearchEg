#include "Tcp_server.h"
#include "Tools.h"

Tcp_server::Tcp_server(const string &ip, unsigned short port, Engine_basic *engine)
    : _acceptor(ip, port), _ioserver(), _acceptserver(_acceptor, _ioserver), _pool(10), _engine(engine)
{
    // vector<string> res = _engine->recommendWord("你");
    // std::cout<<"候选词：";
    // for(auto&elem:res)
    // {
    //     std::cout<<elem<<",";
    // }
    // std::cout<<"\n";
}

Tcp_server::~Tcp_server()
{
}
void Tcp_server::Start()
{
    _acceptor.ready();
    _pool.Start();

    setFunction();
    std::thread th1(std::bind(&IOserver::loop, &_ioserver));
    _acceptserver.Start();
    th1.join();
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
    // string msg = recv.method;
    if (recv.method == "GET")
    {
        // 将收到数据和TCP对象打包
        HttpPackge pack("/home/lzc/SearchEg/data/search.html", con);
        // 将任务包发给子线程(con、msg)
        addTask(_pool, pack);
        std::cout << "返回一个网页" << "\n";
    }
    else
    {
        std::cout<<"请求方法"<<recv.method<<","<<"URL"<<recv.url<<"\n";
    }
    // 1、HTTP GET任务则打包成获取网页任务
    // 2、HTTP PUT任务则打包成搜索任务

    // 将收到数据和TCP对象打包
    // Packge pack(msg, con);
    // 将任务包发给子线程(con、msg)
    // addTask(_pool, pack);
}
void Tcp_server::onNewConnection(const Tcpconnection_ptr &con)
{
    cout << con->toString() << " has connected!!!" << endl;
}
void Tcp_server::onClose(const Tcpconnection_ptr &con)
{
    cout << con->toString() << " has closed!!!" << endl;
}
