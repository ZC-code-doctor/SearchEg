#ifndef __RECOMMEND_H__
#define __RECOMMEND_H__

#include "Configuration.h"
#include "WebPage.h"
#include "Tools.h"
#include "LRUcache.h"


#include <memory>
#include <map>
#include <map>
#include <string>
#include <set>
#include <cppjieba/Jieba.hpp>
#include <mutex>


using std::mutex;
using std::shared_ptr;
using std::map;
using std::map;
using std::string;
using std::set;
using std::pair;


class Engine_basic
{
public:
    Engine_basic()=default;
    virtual ~Engine_basic();
    virtual void loadResoure()=0;
    virtual vector<json> SearchPage(const string&)=0;
    virtual vector<string> recommendWord(const string&)=0;
private:
    //禁止拷贝构造、赋值运算符函数、移动语义
    Engine_basic(const Engine_basic&);
    Engine_basic(const Engine_basic&&);
    const Engine_basic& operator=(const Engine_basic& rhs);
    const Engine_basic& operator=(const Engine_basic&& rhs);
};


class Engine
:public Engine_basic
{
public:
    Engine(Configuration* ,shared_ptr<cppjieba::Jieba>);
    ~Engine();

    //加载资源
    virtual void loadResoure()override;
    //推荐候选词
    virtual vector<string> recommendWord(const string&)override;
    //匹配网页
    virtual vector<json> SearchPage(const string&)override;
    //引擎缓存相关,更新引擎缓存
    void updatCache();
private:
    //分割字符串类
    size_t getByteNum_UTF8(const char byte);
    vector<string> spiltWord(const string&);
    //网页搜索
    vector<set<pair<int, double>>> getDocidSet(const vector<string>& );
    vector<double> KeyWord_TFIDF(const vector<string>& word);
    map<int,vector<double>> get_FileVecotr(const vector<string>& SearchWord,int ,double threshold);
    vector<pair<string,string>> getFile(const map<double,int>&);
    vector<string> cleanKeyWord(vector<string>& );

private:
    Configuration* _pConf;
    shared_ptr<cppjieba::Jieba> _jieba;
    //词库表
    vector<pair<string,int>>* _yuliaoTable;
    map<string,set<int>>* _indexTable;
    //网页表
    map<int,pair<int,int>>* _offsetTable;
    map<string,set<pair<int,double>>>* _invertIndexTable;
    //引擎缓存
    CacheManager _cacheManager;
};









#endif