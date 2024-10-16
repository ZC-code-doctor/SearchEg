#ifndef __INETADDERESS_H__
#define __INETADDERESS_H__

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::string;

class InetAdderess
{
public:
    InetAdderess(const string& ip,unsigned short port);
    InetAdderess(const struct sockaddr_in&);
    ~InetAdderess();
    string get_ip();
    unsigned short get_port();
    struct sockaddr_in* get_sockaddr();

private:
    struct sockaddr_in _addr;

};

#endif
