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
#include <thread>  
#include <mutex>

#include "nlohmann/json.hpp"


using json = nlohmann::json;
using std::map;
using std::vector;
using std::unordered_map;
using std::list;
using std::string;
using std::pair;


class LRUcache
{
public:
    LRUcache(int capacity=3);
    ~LRUcache();
    void addElement(const string &key,vector<json> value);
    bool readCache(const string& keyWords , vector<json>& );
    //缓存处理相关
    void Clear();
    void updataExange(const LRUcache &rhs);
    void updata();
    list<pair<string,vector<json>>> & getPendingUpdateList();
private:
    size_t _capacity;
    list<pair<string,vector<json>>> _pendingUpdateList;
    //对外交互缓存
    unordered_map<string, list<vector<json>>::iterator>* _hashMap;
    list<vector<json>>* _resultsList;
    //与共享内存交互缓存
    unordered_map<string, list<vector<json>>::iterator>* _exchangeHashMap;
    list<vector<json>>* _exchangeList;  
};


class CacheManager
{
public:
    ~CacheManager();
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