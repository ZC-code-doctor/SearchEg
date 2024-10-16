#include "LRUcache.h"
#include <iostream>




LRUcache::LRUcache(int capacity)
    : _capacity(capacity)
    , _hashMap(new unordered_map<string, list<vector<json>>::iterator>())
    , _resultsList(new list<vector<json>>())
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
    std::cout << "~LRUcache" << "\n";
}
void LRUcache::addElement(const string &key, vector<json> value)
{

    
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
        std::cout << "存在节点过期\n";
    }

    // 无论是否删除节点，均插入新内容到链表末尾
    _resultsList->push_back(value);

    // 更新键对应的迭代器
    (*_hashMap)[key] = --_resultsList->end();
}
bool LRUcache::readCache(const string &keyWords, vector<json> &jsonSet)
{
    // 在 _hashMap 中查找 keyWords
    auto it = _hashMap->find(keyWords);

    // 如果找到了 keyWords
    if (it != _hashMap->end())
    {
        // 将传入参数修改为查到的内容
        jsonSet = *it->second;

        // 找到缓存中的值，更新到最近使用（移动到 list 的尾部）
        _resultsList->splice(_resultsList->end(), (*_resultsList), it->second);
        // 返回 true 表示命中缓存
        return true;
    }
    // 如果没有找到，返回 false 表示缓存未命中
    return false;
}


// LRU管理类
// 根据线程获取对应的LRUcache对象
LRUcache* CacheManager::getCache(std::thread::id threadId)
{
    // 检查 map 中是否有对应线程 ID 的缓存
    if (_cacheList.find(threadId) == _cacheList.end())
    {
        // 如果没有找到，使用 emplace 创建一个新的 LRUcache，并插入 map
        _cacheList.emplace(threadId, new LRUcache(3)); // 假设容量为 3
    }
    // 返回线程对应的缓存
    return _cacheList[threadId];
}
void CacheManager::periodicUpdateCaches()
{

}






//暂时保存LRU类
/*
LRUcache::LRUcache(int capacity)
    : _capacity(capacity), _hashMap(new unordered_map<string, list<vector<json>>::iterator>()), _resultsList(new list<vector<json>>())
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
    std::cout << "~LRUcache" << "\n";
}
void LRUcache::addElement(const string &key, vector<json> value)
{
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
        std::cout << "存在节点过期\n";
    }

    // 无论是否删除节点，均插入新内容到链表末尾
    _resultsList->push_back(value);

    // 更新键对应的迭代器
    (*_hashMap)[key] = --_resultsList->end();
}
bool LRUcache::readCache(const string &keyWords, vector<json> &jsonSet)
{
    // 在 _hashMap 中查找 keyWords
    auto it = _hashMap->find(keyWords);

    // 如果找到了 keyWords
    if (it != _hashMap->end())
    {
        // 将传入参数修改为查到的内容
        jsonSet = *it->second;

        // 找到缓存中的值，更新到最近使用（移动到 list 的尾部）
        _resultsList->splice(_resultsList->end(), (*_resultsList), it->second);
        // 返回 true 表示命中缓存
        return true;
    }
    // 如果没有找到，返回 false 表示缓存未命中
    return false;
}
*/
