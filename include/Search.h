#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "Configuration.h"
#include "WebPage.h"

#include <unordered_map>
#include <string>
#include <set>

using std::unordered_map;
using std::string;
using std::pair;
using std::set;



class Search_base
{
public:
    virtual ~Search_base()=0;
    virtual void SearchPage()=0;
    virtual void loadResoure()=0;

private:
};


class Search
:public Search_base
{
public:

    Search(Configuration*);
    ~Search();

    virtual void SearchPage()override;
    //加载资源文件
    virtual void loadResoure()override;

private:
    Configuration* _pConf;
    unordered_map<int,pair<int,int>>* _offsetTable;
    unordered_map<string,set<pair<int,double>>>* _invertIndexTable;
};


#endif