#include "ThreadPool.h"
#include "Task.h"




ThreadPool::ThreadPool(size_t cap)
:isExit(false)    
,_capacity(cap)
,_taskQue(cap)
,Pool()
,_factory()
{

}
ThreadPool::~ThreadPool()
{

}




void ThreadPool::doTask()
{
    while(!isExit)
    {
        Task* task=getTask();
        if(task)
        {
            task->producess();
            delete task;
        }
        else
        {
            cout<<"This is nullptr"<<endl;
        }
    }
}
void ThreadPool::Start()
{
    for(size_t idx=0;idx<_capacity;++idx)
    {
        Pool.emplace_back(&ThreadPool::doTask,this);
    }

}
void ThreadPool::Stop()
{
    while(!_taskQue.isEmpty());
    isExit = true;
    _taskQue.wakeUp();
    for(auto &th : Pool)
    {
        th.join();
    }
}
Task* ThreadPool:: getTask()
{
    return _taskQue.pop();
}
