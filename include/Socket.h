#ifndef __SOCKET_H__
#define __SOCKET_H__



#include <sys/types.h>              
#include <sys/socket.h>
#include <unistd.h>

class Socket
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int get_fd();

private:

    int _fd;
};




#endif
