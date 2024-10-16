#include "TaskQueue.h"


TaskQueue::TaskQueue(size_t cap)
:_mtx()
,Full()
,Empty()
,_que()
,_cap(cap)
,flag(true)
{

}


TaskQueue::~TaskQueue()
{
    
}

void TaskQueue::push(Task* task)
{
    //上锁
    std::unique_lock<mutex> mtx(_mtx);
    while(isFull())
    {
        //队列满，线程在Full条件变量下等待
        Full.wait(mtx);
    }
    _que.push(task);
    Empty.notify_one();
}


Task* TaskQueue::pop()
{
    std::unique_lock<mutex> mtx(_mtx);
    while(isEmpty() && flag)
    {
        //队列空，线程在Empty条件变量下等待
        Empty.wait(mtx);
    }
    if(flag)
    {
        Task* temp = _que.front();
        _que.pop();
        Full.notify_one();
        return temp;
    }
    else
    {
        return nullptr;
    }
}

void TaskQueue::wakeUp()
{
    flag=false;
    Empty.notify_all();
}

bool TaskQueue::isFull()
{
    return _que.size()==_cap;
}
bool TaskQueue::isEmpty()
{
    return 0==_que.size();
}
