#include "Acceptor.h"



Acceptor::Acceptor(const string& ip,unsigned short port)
:_Inetaddr(ip,port)
,_sock()
{

}

Acceptor::~Acceptor()
{

}

void Acceptor::ready()
{
    int opt=1;
    setsockopt(_sock.get_fd(),SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
    setsockopt(_sock.get_fd(),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind();
    listen();

}

int Acceptor::accept()
{
   int peerfd=::accept(_sock.get_fd(),nullptr,nullptr); 
   return peerfd;
}

void Acceptor::bind()
{
    ::bind(_sock.get_fd(),(sockaddr*)_Inetaddr.get_sockaddr(),sizeof(_Inetaddr));
}

void Acceptor::listen()
{
    ::listen(_sock.get_fd(),1024);
}


int Acceptor::get_fd()
{
    return _sock.get_fd();
}

