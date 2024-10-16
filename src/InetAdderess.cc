#include "InetAdderess.h"

InetAdderess::InetAdderess(const string& ip,unsigned short port)
{
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
}


InetAdderess::InetAdderess(const struct sockaddr_in& addr)
:_addr(addr)
{

}


InetAdderess::~InetAdderess()
{

}


string InetAdderess::get_ip()
{
    return inet_ntoa(_addr.sin_addr);
}


unsigned short InetAdderess::get_port()
{
    return ntohs(_addr.sin_port);
}

struct sockaddr_in* InetAdderess::get_sockaddr()
{
    return &_addr;
}
