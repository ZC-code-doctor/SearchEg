#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__


#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

class SocketIO
{
public:
    explicit SocketIO(int fd); 
    ~SocketIO();
    int readn(char* buf,int len);
    int writen(const char* buf,int len);
    int readLine(char* buf,int len);
    int readHttpRequest(char* buf, int len);
private:
    int _fd;

};

#endif
