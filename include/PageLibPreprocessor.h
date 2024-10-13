#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "simhash/Simhasher.hpp"

#include <vector>
#include <unordered_map>
#include <string>

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
    PageLibPreprocessor(Configuration*);
    ~PageLibPreprocessor();
    void initPageLib(Configuration *pConf);
private:
    //辅助函数
    int HammingDistance(uint64_t hash1, uint64_t hash2);
    bool CompareTexts(const std::string& text1, const std::string& text2, const Simhasher& simhasher);
private:

    vector<WebPage> _pageLib;                                           //网页库
    unordered_map<int,pair<int,int>> _offsetLib;                        //网页偏移库
    unordered_map<string,vector<pair<int,double>>> _invertIndexTable;   //倒排索引表
};












#endif