#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include "Factory.h"

#include <vector>
#include <thread>
#include <string>

using std::string;
using std::vector;
using std::thread;


class ThreadPool
{
public:
    ThreadPool(size_t cap);
    ~ThreadPool();
    

template <class T>
friend void addTask(ThreadPool& pool,T val);
    void Start();
    void Stop();

private:
    void doTask();
    Task* getTask();
private:
    bool isExit;
    size_t _capacity;
    TaskQueue _taskQue;
    vector<thread> Pool;
    Factory _factory;
};


template <class T>
void addTask(ThreadPool& pool,T val)
{
    Task* temp= pool._factory.productTask(val);
    pool._taskQue.push(temp);
}

#endif
