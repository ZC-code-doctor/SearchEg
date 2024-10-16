#ifndef __TASK_H__
#define __TASK_H__

#include "Tcpconnection.h"
#include "Engine.h"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;
class Task{
public:
    virtual void producess()=0;
    virtual ~Task()=0;
};

class TaskA
:public Task
{
public:
    TaskA(string);
    void producess();
    ~TaskA();
private:
    string name;
};

class TaskB
:public Task
{
public:
    void producess();
    TaskB(int num);
    ~TaskB();
private:
    int number;
};

struct Packge
{
    Packge(string& msg, Tcpconnection_ptr con);
    string _msg;
    Tcpconnection_ptr _conn;  
};

class TaskC
:public Task
{
public:
    TaskC(Packge& pack);
    void producess();
private:
   Packge _pack;
};

class TaskD
:public Task
{
public:
    TaskD(factor&& cb);
    void producess();
private:
   factor _cb; 
};


//本项目的任务类型
struct HttpPackge
{
    HttpPackge(string msg, Tcpconnection_ptr con);
    string _msg;
    Tcpconnection_ptr _conn;  
};

class HttpTask
:public Task
{
public:
    HttpTask(HttpPackge packge);
    void producess();
private:
   HttpPackge _packge; 
};

struct SearchPackge
{
    SearchPackge(string msg, Engine_basic* engine,Tcpconnection_ptr con);
    string _keyWord;
    Engine_basic* _engine;
    Tcpconnection_ptr _conn;  
};

class SearchTask
:public Task
{
public:
    SearchTask(SearchPackge packge);
    void producess();
private:
    SearchPackge _packge; 
};

struct RecommendPackge
{
    RecommendPackge(string msg, Engine_basic* engine,Tcpconnection_ptr con);
    string _keyWord;
    Engine_basic* _engine;
    Tcpconnection_ptr _conn;  
};

class RecommendTask
:public Task
{
public:
    RecommendTask(RecommendPackge packge);
    void producess();
private:
    RecommendPackge _packge; 
};


#endif
