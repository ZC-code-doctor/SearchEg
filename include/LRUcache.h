#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <string>
#include <list>

using std::list;
using std::string;

class LRU_BASE
{
public:
    virtual ~LRU_BASE()=default;
    virtual void addElement(const string &key, const string& value)=0;
    virtual void readFromFile(const string & filename)=0;
    virtual void writeToFile(const string & filename)=0;
    
    //virtual void update(const LRU_BASE & rhs);
    //virtual list<string,string> & getPendingUpdateList();
private:

};








#endif