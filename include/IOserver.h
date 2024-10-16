#ifndef __IOSERVER_H__
#define __IOSERVER_H__

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <functional>
#include <sys/eventfd.h>
#include <mutex>
#include "ThreadPool.h"

using std::function;
using std::shared_ptr;
using std::vector;
using std::map;
using std::mutex;
using std::cout;
using std::endl;

class Tcpconnection;

using Tcpconnection_ptr=std::shared_ptr<Tcpconnection>;
using Callback = function<void(const Tcpconnection_ptr &)>;
using factor = function<void()>;


class IOserver
{
public:
    IOserver();
    ~IOserver();

    void loop();
    void unloop();
    void Epoll_wait();
    void handlMessage(int fd);
    void handlNewconnection(int fd);
    void addEpoll(int fd);
    void delEpoll(int fd);

    void setCallbackClose(Callback&&);
    void setCallbackMessage(Callback&&);
    void setCallbackNewConnect(Callback&&);

    void addFactor(factor&&);
    //eventfd线程通信相关操作
    void wakeup();

private:
    void doPendings();
    int createEventfd();
    void handleRead();
private:
    int _epfd;
    vector<struct epoll_event> _evtList;
    bool _isLoop;
    map<int,Tcpconnection_ptr> _conns;
    Callback _handleClose;
    Callback _handleMessage;
    Callback _handleNewConnect;
    //eventfd线程通信的文件描述符
    int _evtfd;
    vector<factor> _pendings;
    mutex _mtx;
    ThreadPool _sendPool;
};

#endif
