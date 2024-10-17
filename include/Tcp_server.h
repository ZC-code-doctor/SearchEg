#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.h"
#include "AcceptServer.h"
#include "IOserver.h"
#include "ThreadPool.h"
#include "Engine.h"

#include <functional>

using std::bind;

class Tcp_server
{
public:
    Tcp_server(const string& ip, unsigned short port,Engine* engine);
    ~Tcp_server();
    void Start();
    void Stop();
private:
    void setFunction();
    void onMessage(const Tcpconnection_ptr &con);
    void onNewConnection(const Tcpconnection_ptr &con);
    void onClose(const Tcpconnection_ptr &con);
private:
    Acceptor _acceptor;
    IOserver _ioserver;         //监听已有的连接是否发送了请求
    AcceptServer _acceptserver; //建立新的连接
    ThreadPool _pool;           //工作线程池，处理业务
    Engine* _engine;            //搜索引擎
};

#endif
