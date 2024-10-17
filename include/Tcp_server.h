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
    IOserver _ioserver;
    AcceptServer _acceptserver;
    ThreadPool _pool;
    Engine* _engine;

};

#endif
