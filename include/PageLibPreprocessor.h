#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "simhash/Simhasher.hpp"
#include "cppjieba/Jieba.hpp"

#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <set>

using std::set;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;
using std::pair;
using simhash::Simhasher;

class WebPage;
class Configuration;

class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration*,cppjieba::Jieba*);
    ~PageLibPreprocessor();

private:
    void initPageLib(Configuration *pConf);
    void initInverLib();
    void store();

    //辅助函数
    bool CompareTexts(const std::string& text1, uint64_t& fileHash ,const Simhasher& simhasher);
    // 切割文章
    vector<string> split(const string& text);
private:

    vector<WebPage> _pageLib;                                           //网页库
    unordered_map<int,pair<int,int>> _offsetLib;                        //网页偏移库
    unordered_map<string,set<pair<int,double>>> _invertIndexTable;      //倒排索引表
    vector<uint64_t> _testHash;                                         //文章hash指纹
    cppjieba::Jieba* _jieba;
};

int HammingDistance(uint64_t hash1, uint64_t hash2);












#endif