#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAdderess.h"

#include <string.h>
#include <sstream>
#include <memory>
#include <functional>
#include <iostream>

using std::function;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::ostringstream;

class IOserver;
using factor = function<void()>;

class Tcpconnection;
using Tcpconnection_ptr=std::shared_ptr<Tcpconnection>;

class Tcpconnection
:public std::enable_shared_from_this<Tcpconnection>
{
using Callback = function<void(const Tcpconnection_ptr &)>;
public:
    explicit Tcpconnection(int fd,IOserver* ioserver);
    ~Tcpconnection();
    string recvMsg();
    int sendMsg(const string& msg);
    string toString();
    
    void setCallbackClose(const Callback&);
    void setCallbackMessage(const Callback&);
    void setCallbackNewConnect(const Callback&);

    void handleClose();
    void handleMessage();
    void handleNewConnect();
    
    void addEventPending(factor&&);
    bool isClose();
private:
    struct sockaddr_in get_local(int fd);
    struct sockaddr_in get_peer(int fd);

private:
    Socket _sock;
    SocketIO _sockIO;
    InetAdderess _local;
    InetAdderess _peer;

    Callback _handleClose;
    Callback _handleMessage;
    Callback _handleNewConnect;
    
    IOserver* _ioserver;

};

#endif
