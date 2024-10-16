#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include <mutex>
#include <condition_variable>
#include <queue>

using std::mutex;
using std::condition_variable;
using std::queue;

class Task;

class TaskQueue
{
public:
    TaskQueue(size_t cap);
    ~TaskQueue();

    void push(Task*);
    Task* pop();
    bool isEmpty();
    void wakeUp();

private:
    bool isFull();


private:
    mutex _mtx;
    condition_variable Full;
    condition_variable Empty;
    queue<Task*> _que;
    size_t _cap;
    bool flag;
    

};

#endif
