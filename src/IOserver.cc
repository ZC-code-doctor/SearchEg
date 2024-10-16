#include "IOserver.h"
#include "Acceptor.h"
#include "Tcpconnection.h"


IOserver::IOserver()
:_epfd(epoll_create1(0))
,_evtList(1024)
,_isLoop(false)
,_conns()
,_evtfd(createEventfd())
,_pendings()
,_mtx()
,_sendPool(10)
{
    //启动线程池
    _sendPool.Start();
    //将线程通信的evtfd加入到epoll中监听
    addEpoll(_evtfd);
}

IOserver::~IOserver()
{
    close(_epfd);
    close(_evtfd);
}

void IOserver::loop()
{
    _isLoop=true;
    while(_isLoop)
    {
        Epoll_wait();
    }
}

void IOserver::unloop()
{
    _isLoop = false;
    _sendPool.Stop();
}
void IOserver::Epoll_wait()
{

    int nready=0;
    do
    {
     nready=epoll_wait(_epfd,&_evtList[0],1024,3000);
    }while(nready==-1&&errno==EINTR);
    if(nready==0)
    {
        cout<<"Timewait over"<<endl;
    }
    else
    {
        for(int idx=0;idx<nready;++idx)
        {
            int fd=_evtList[idx].data.fd;
            if(fd == _evtfd)
            {
                //说明子线程业务处理完毕，通知主线程进行处理
                handleRead();
                doPendings();
            }
            else
            {
                //处理老连接的请求
                if(EPOLLIN & _evtList[idx].events)
                {
                    handlMessage(fd);
                }
            }
        }
    }

}
void IOserver::handlMessage(int fd)
{
    auto it = _conns.find(fd);
    //接收消息
    if(it!=_conns.end())
    {
        bool flag=it->second->isClose();
        if(flag)
        {
            //连接断开
            it->second->handleClose();
            delEpoll(fd);
            _conns.erase(it);
        }
        else
        {
            it->second->handleMessage();
        }
    }
    else
    {
        cout<<"该连接不存在"<<endl;
        return ;
    }
}
void IOserver::handlNewconnection(int fd)
{

    Tcpconnection_ptr conn(new Tcpconnection(fd,this));
    _conns.insert({fd,conn});


    conn->setCallbackNewConnect(_handleNewConnect);
    conn->setCallbackClose(_handleClose);
    conn->setCallbackMessage(_handleMessage);

    addEpoll(fd);
    conn->handleNewConnect();

}
void IOserver::addEpoll(int fd)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=fd;
    //监听读事件
    ev.events=EPOLLIN;
    epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);

}
void IOserver::delEpoll(int fd)
{
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=fd;
    //监听读事件
    ev.events=EPOLLIN;
    epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
}


//三个注册函数
void IOserver::setCallbackClose(Callback&& Callback_close)
{
   _handleClose=std::move(Callback_close); 
}
void IOserver::setCallbackMessage(Callback&& Callback_Message)
{
    _handleMessage = std::move(Callback_Message); 
}
void IOserver::setCallbackNewConnect(Callback&& Callback_NewConnect)
{
    _handleNewConnect = std::move(Callback_NewConnect);
}


void IOserver::wakeup()
{
    uint64_t u=1;
    ssize_t ret = write(_evtfd,&u,sizeof(uint64_t));
    if(ret!=sizeof(uint64_t))
    {
        perror("handleRead error\n");
        return;
    }

}
int IOserver::createEventfd()
{
    int fd= eventfd(1,0);
    if(fd<0)
    {
        perror("createEventfd Error\n");
        return -1;
    }
    return fd;
}
void IOserver::handleRead()
{
    uint64_t u=1;
    ssize_t ret = read(_evtfd,&u,sizeof(uint64_t));
    if(ret!=sizeof(uint64_t))
    {
        perror("handleRead error\n");
        return;
    }
}

void IOserver::doPendings()
{
    vector<factor> temp;
    {
        std::lock_guard<mutex> lg(_mtx);
        temp.swap(_pendings);
    }
    for(auto& cb:temp)
    {
        if(cb)
        {
            addTask(_sendPool,cb);
        }
    }
}
void IOserver::addFactor(factor&& cb)
{
    {
        std::lock_guard<mutex> lg(_mtx);
        _pendings.emplace_back(cb);
    }
    wakeup();
}



