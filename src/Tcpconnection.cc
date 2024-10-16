#include "Tcpconnection.h"
#include "IOserver.h"

Tcpconnection::Tcpconnection(int fd,IOserver* ioserver)
:_sock(fd)
,_sockIO(fd)
,_local(get_local(fd))
,_peer(get_peer(fd))
,_ioserver(ioserver)
{

}


Tcpconnection::~Tcpconnection()
{

}


string Tcpconnection::recvMsg()
{
    char buff[4096]={0};
    _sockIO.readHttpRequest(buff,4096);
    return buff;
}

int Tcpconnection::sendMsg(const string& msg)
{
    int ret=_sockIO.writen(msg.c_str(),msg.size());
    return ret;
}


struct sockaddr_in Tcpconnection::get_local(int fd)
{
    sockaddr_in temp;
    memset(&temp,0,sizeof(temp));
    socklen_t opt=sizeof(temp);
    getsockname(fd,(struct sockaddr*)&temp,&opt);
    return temp;
    
}
struct sockaddr_in Tcpconnection::get_peer(int fd)
{
    sockaddr_in temp;
    memset(&temp,0,sizeof(temp));
    socklen_t opt=sizeof(temp);
    getpeername(fd,(struct sockaddr*)&temp,&opt);
    return temp;

}
string Tcpconnection::toString()
{
    ostringstream oss;
    oss << _local.get_ip() << ":"
        << _local.get_port() << "---->"
        << _peer.get_ip() << ":"
        << _peer.get_port();

    return oss.str();    
}

//注册三个事件
void Tcpconnection::setCallbackClose(const Callback& cb)
{
    _handleClose=cb;
}
void Tcpconnection::setCallbackMessage(const Callback& cb)
{
    _handleMessage=cb;
}
void Tcpconnection::setCallbackNewConnect(const Callback& cb)
{
    _handleNewConnect = cb;
}

//执行三个注册事件
void Tcpconnection::handleClose()
{
    if(_handleClose)
    {
        _handleClose(shared_from_this());
    }
    else
    {
        cout<<"Callback_close == nullptr"<<endl;
    }
}
void Tcpconnection::handleMessage()
{
    if(_handleMessage)
    {
        _handleMessage(shared_from_this());
    }
    else
    {
        cout<<"Callback_Message == nullptr"<<endl;
    }


}
void Tcpconnection::handleNewConnect()
{
    if(_handleNewConnect)
    {
        _handleNewConnect(shared_from_this());
    }
    else
    {
        cout<<"Callback_NewConnect == nullptr"<<endl;
    }

}

bool Tcpconnection::isClose() 
{
    char buf[10]={0};
    int ret=recv(_sock.get_fd(),buf,sizeof(buf),MSG_PEEK);
    return ret==0;
}

void Tcpconnection::addEventPending(factor&& cb)
{

    _ioserver->addFactor(std::move(cb));
}
