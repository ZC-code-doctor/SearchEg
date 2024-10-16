#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "InetAdderess.h"
#include "Socket.h"

#include <sys/types.h>          
#include <sys/socket.h>


#include <string>

using std::string;

class Acceptor
{
public:
    Acceptor(const string& ip,unsigned short port);
    ~Acceptor();
    void ready();
    int accept();
    int get_fd();
private:
    void bind();
    void listen();
private:
    InetAdderess _Inetaddr;
    Socket _sock;

};

#endif
