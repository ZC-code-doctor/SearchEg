#ifndef __FACTORY_H__
#define __FACTORY_H__


#include "Task.h"

#include <string>
using std::string;


class Task;

class Factory
{
public:
    Task* productTask(int);
    Task* productTask(string);
    Task* productTask(Packge);
    Task* productTask(factor);
    Task* productTask(HttpPackge);

private:

};


#endif
