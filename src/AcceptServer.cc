#include "AcceptServer.h"
#include "IOserver.h"
#include "Acceptor.h"


AcceptServer::AcceptServer(Acceptor& accpetor,IOserver& ioserver)
:_acceptor(accpetor)
,_ioserver(ioserver)
,_isLoop(false)
{

}
AcceptServer::~AcceptServer()
{

}
void AcceptServer::Start()
{
    _isLoop=true;
    while(_isLoop)
    {
       int fd =  _acceptor.accept();
       if(fd<0)
       {
           perror("accept error\n");
           return ;
       }
        _ioserver.handlNewconnection(fd);
    }
}
void AcceptServer::Stop()
{
    _isLoop=false;
}

