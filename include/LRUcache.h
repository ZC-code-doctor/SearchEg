#ifndef __LRUCACHE_H__
#define __LRUCACHE_H__

#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <map>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <thread>  // for std::this_thread::get_id

#include "nlohmann/json.hpp"

using json = nlohmann::json;
using std::map;
using std::vector;
using std::unordered_map;
using std::list;
using std::string;


class LRUcache
{
public:
    // LRUcache()=default;
    LRUcache(int capacity=3);
    ~LRUcache();
    void addElement(const string &key,vector<json> value);
    bool readCache(const string& keyWords , vector<json>& );
    void updata(const LRUcache& rhs);
    list<vector<json>> & getPendingUpdateList();

private:
    int _capacity;
    list<vector<json>> _pendingUpdateList;
    unordered_map<string, list<vector<json>>::iterator>* _hashMap;
    list<vector<json>>* _resultsList;
};


class CacheManager
{
public:
    LRUcache* getCache(std::thread::id idx);
    void periodicUpdateCaches();
private:
    //线程id对应一个LRU实例,存放堆上的指针
    map<std::thread::id,LRUcache*> _cacheList;
};




//暂时保存
/*
class LRUcache
{
public:
    LRUcache(int capacity);
    ~LRUcache();
    void addElement(const string &key,vector<json> value);
    bool readCache(const string& keyWords , vector<json>& );

private:
    int _capacity;
    unordered_map<string, list<vector<json>>::iterator>* _hashMap;
    list<vector<json>>* _resultsList;

};
*/


#endif