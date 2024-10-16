#include "SocketIO.h"


SocketIO::SocketIO(int fd)
:_fd(fd)
{

}

SocketIO::~SocketIO()
{

}

//接收数据
int SocketIO::readn(char* buf,int len)
{
    int left=len;
    while(left>0)
    {
        int ret=read(_fd,buf,left);
        if(ret==-1 && errno==EINTR)
        {
            continue;
        }
        else if(ret==-1)
        {
            return -1;
        }
        else if(ret==0)
        {
            break;//对端关闭
        }
        else
        {
            buf+=ret;
            left-=ret;
        }
    }
    return len-left;
}

//发生数据
int SocketIO::writen(const char* buf,int len)
{
    int left = len;
    const char *pstr = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = write(_fd, pstr, left);
        if(-1 == ret && errno == EINTR)
        {
            continue;
        }
        else if(-1 == ret)
        {
            return -1;
        }
        else if(0 == ret)
        {
            break;
        }
        else
        {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}

int SocketIO::readLine(char* buf,int len)
{
    int left=len;
    int total=0;
    while(left>0)
    {
        int ret=recv(_fd,buf,left,MSG_PEEK);
        if(ret==-1&&errno==EINTR)
        {
            continue;
        }
        else if(ret==-1)
        {
            return -1;
        }
        else
        {
            for(int idx=0;idx<ret;++idx)
            {
                if(buf[idx]=='\n')
                {
                    int sz=idx+1;
                    //设置C风格字符串终止符
                    buf[sz]='\0';
                    readn(buf,sz);
                    total+=idx; 
                    return len-total; 
                }
            }
            readn(buf,ret);
            buf+=ret;   //指针偏移
            left-=ret;  //剩余字符数计算
            total+=ret; //总计接收字符数
        }
    }
    return total;
}
