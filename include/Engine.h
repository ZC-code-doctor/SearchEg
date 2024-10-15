#ifndef __RECOMMEND_H__
#define __RECOMMEND_H__

#include "Configuration.h"
#include "WebPage.h"
#include "Tools.h"


#include <memory>
#include <map>
#include <map>
#include <string>
#include <set>
#include <cppjieba/Jieba.hpp>

using std::shared_ptr;
using std::map;
using std::map;
using std::string;
using std::set;
using std::pair;


class Engine_basic
{
public:
    virtual ~Engine_basic();
    virtual void loadResoure()=0;
    virtual WebPage SearchPage(const string&)=0;
    virtual vector<string> recommendWord(const string&)=0;
private:

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
    virtual WebPage SearchPage(const string&)override;
private:
    //分割字符串类
    size_t getByteNum_UTF8(const char byte);
    vector<string> spiltWord(const string&);
    //网页搜索
    vector<set<pair<int, double>>> getDocidSet(const vector<string>& );

private:
    Configuration* _pConf;
    shared_ptr<cppjieba::Jieba> _jieba;

    vector<pair<string,int>>* _yuliaoTable;
    map<string,set<int>>* _indexTable;

    map<int,pair<int,int>>* _offsetTable;
    map<string,set<pair<int,double>>>* _invertIndexTable;
};









#endif