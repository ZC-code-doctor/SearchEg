#include "LRUcache.h"
#include <iostream>
#include <mutex>

// 定义一个静态的全局互斥锁，对其它文件屏蔽
//仅在LRU缓存处理时可以使用
static std::mutex cacheMutex; 

LRUcache::LRUcache(int capacity)
    : _capacity(capacity)
    , _hashMap(new unordered_map<string, list<vector<json>>::iterator>())
    , _resultsList(new list<vector<json>>())
    ,_exchangeHashMap(new unordered_map<string, list<vector<json>>::iterator>())
    ,_exchangeList(new list<vector<json>>())
{

}
LRUcache::~LRUcache()
{
    if (_hashMap != nullptr)
    {
        delete _hashMap;
        _hashMap = nullptr;
    }
    if (_resultsList != nullptr)
    {
        delete _resultsList;
        _resultsList = nullptr;
    }
    if(_exchangeHashMap!=nullptr)
    {
        delete _exchangeHashMap;
        _exchangeHashMap =nullptr;
    }
    if(_exchangeList != nullptr)
    {
        delete _exchangeList;
        _exchangeList = nullptr;
    }
    std::cout << "~LRUcache" << "\n";
}


void LRUcache::addElement(const string &key, vector<json> value)
{
    // 首先检查是否已经存在相同的键
    auto it = _hashMap->find(key);
    if (it != _hashMap->end())
    {
        // 如果键已经存在，则更新值并将其移动到链表的末尾
        *(it->second) = value; // 更新值
        _resultsList->splice(_resultsList->end(), *_resultsList, it->second); // 移动到末尾

        // 同时更新 pending 列表中的该元素
        for (auto &pending : _pendingUpdateList)
        {
            if (pending.first == key)
            {
                pending.second = value;
                break;
            }
        }
        return; // 已处理完，返回
    }

    // 如果缓存已满，删除最老的元素
    if (_resultsList->size() >= _capacity)
    {
        // 找到与第一个节点对应的键
        for (auto it = _hashMap->begin(); it != _hashMap->end(); ++it)
        {
            if (it->second == _resultsList->begin())
            {
                _hashMap->erase(it); // 删除过期的键
                break;
            }
        }
        // 缓存满，则删除第一个节点
        _resultsList->pop_front();
        std::cout << "缓存已满，删除最老的节点\n";
    }

    // 插入新内容到链表末尾
    _resultsList->push_back(value);

    // 加入一份到 pending 中准备更新
    _pendingUpdateList.push_back(std::make_pair(key, value));

    // 更新键对应的迭代器
    (*_hashMap)[key] = --_resultsList->end();
}
bool LRUcache::readCache(const string &keyWords, vector<json> &jsonSet)
{
    // 检查 _hashMap 和 _resultsList 是否为空
    if (!_hashMap || !_resultsList)
    {
        std::cerr << "Error: _hashMap or _resultsList is not initialized." << std::endl;
        return false;
    }

    // 在 _hashMap 中查找 keyWords
    auto it = _hashMap->find(keyWords);

    // 如果找到了 keyWords
    if (it != _hashMap->end())
    {
        // 检查迭代器是否仍然有效
        if (it->second != _resultsList->end())
        {
            // 将传入参数修改为查到的内容
            jsonSet = *it->second;

            // 找到缓存中的值，更新到最近使用（移动到 list 的尾部）
            _resultsList->splice(_resultsList->end(), (*_resultsList), it->second);
            // 返回 true 表示命中缓存
            return true;
        }
        else
        {
            std::cerr << "Error: Invalid iterator in _hashMap." << std::endl;
            return false;
        }
    }

    // 如果没有找到，返回 false 表示缓存未命中
    return false;
}

list<pair<string, vector<json>>> &LRUcache::getPendingUpdateList()
{
    return _pendingUpdateList;
}
void LRUcache::updataExange(const LRUcache &rhs)
{
    // 清空当前对象的数据之前，确保它们不是空指针
    if (_exchangeList != nullptr)
    {
        delete _exchangeList;
        _exchangeList = nullptr;
    }
    if (_exchangeHashMap != nullptr)
    {
        delete _exchangeHashMap;
        _exchangeHashMap = nullptr;
    }
    // 分配新的内存并深拷贝,和共享缓存交互的缓存需要深拷贝
    _exchangeList = new std::list<std::vector<json>>(*rhs._resultsList);
    _exchangeHashMap = new std::unordered_map<std::string, std::list<std::vector<json>>::iterator>(*rhs._hashMap);

    // 清空并复制待更新列表
    _pendingUpdateList.clear();
}
void LRUcache::updata()
{
    // 这里需要加锁
    std::lock_guard<std::mutex> lock(cacheMutex); 
    std::swap(_resultsList, _exchangeList);
    std::swap(_hashMap, _exchangeHashMap);
}

// LRU管理类
// 获取线程对应的缓存，V4版本这个操作不需要加锁
LRUcache *CacheManager::getCache(std::thread::id threadId)
{
    std::lock_guard<std::mutex> lock(cacheMutex);
    // 检查 map 中是否有对应线程 ID 的缓存
    if (_cacheList.find(threadId) == _cacheList.end())
    {
        // 如果没有找到，使用 emplace 创建一个新的 LRUcache，并插入 map
        _cacheList.emplace(threadId, new LRUcache(5)); // 假设容量为 5
    }
    // 返回线程对应的缓存
    return _cacheList[threadId];
}

//V3版本
/*
void CacheManager::periodicUpdateCaches()
{
    std::lock_guard<std::mutex> lock(cacheMutex); // 加锁保护共享资源
    // 通过线程id来锁定唯一的缓存地址
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "Current thread ID: " << this_id << std::endl;

    // 获取到主线程的pending
    LRUcache *_cache = _cacheList[this_id];
    if (_cache == nullptr)
    {
        std::cerr << "Error: _cacheList for this thread ID is nullptr" << std::endl;
        return; // 直接返回，避免空指针访问
    }

    // 取出所有的缓存并更新
    for (auto &elem : _cacheList)
    {
        if (elem.second == nullptr)
        {
            std::cerr << "Warning: _cacheList contains a nullptr entry for thread ID " << elem.first << std::endl;
            continue; // 跳过空指针的元素
        }

        if (elem.second != _cache)
        {
            for (auto &value : elem.second->getPendingUpdateList())
            {
                // 添加到主线程缓存
                _cache->addElement(value.first, value.second);
            }
        }
    }

    // 更新其他线程的缓存
    for (auto &elem : _cacheList)
    {
        if (elem.second != _cache)
        {
            // 更新内部缓存信息
            elem.second->updata(*_cache);
        }
    }

    std::cout << "完成更新\n";
}
*/

// V4版本这里也不需要加锁
void CacheManager::periodicUpdateCaches()
{
    // 通过线程id来锁定唯一的缓存地址
    std::thread::id this_id = std::this_thread::get_id();
    //std::cout << "Current thread ID: " << this_id << std::endl;

    // 获取到主线程的pending
    LRUcache *_cache = _cacheList[this_id];
    if (_cache == nullptr)
    {
        std::cerr << "Error: _cacheList for this thread ID is nullptr" << std::endl;
        return; // 直接返回，避免空指针访问
    }

    // 取出所有的缓存并更新
    for (auto &elem : _cacheList)
    {
        if (elem.second == nullptr)
        {
            std::cerr << "Warning: _cacheList contains a nullptr entry for thread ID " << elem.first << std::endl;
            continue; // 跳过空指针的元素
        }

        if (elem.second != _cache)
        {

            for (auto &value : elem.second->getPendingUpdateList())
            {
                // 添加到主线程缓存
                _cache->addElement(value.first, value.second);
            }
        }
    }
    // 更新其他线程的缓存
    for (auto &elem : _cacheList)
    {
        if (elem.second != _cache)
        {
            // 更新内部缓存的交换缓存的信息
            elem.second->updataExange(*_cache);
        }
    }
    for (auto &elem : _cacheList)
    {
        if (elem.second != _cache)
        {
            // 更新内部缓存的交换缓存的信息
            elem.second->updata();
        }
    }

    std::cout << "完成更新\n";
}

CacheManager::~CacheManager()
{
    for (auto &elem : _cacheList)
    {
        delete elem.second;
        elem.second = nullptr;
    }
    std::cout << "~CacheManager\n";
}

