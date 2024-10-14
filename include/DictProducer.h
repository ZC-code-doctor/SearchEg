#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "SplitZh.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <unordered_map>

using std::ofstream;
using std::unordered_map;
using std::map;
using std::set;
using std::vector;
using std::string;
using std::pair;

class SplitTool;
class SplitEn;
class Configuration;

class DictProducer
{
public:
    DictProducer(Configuration*,cppjieba::Jieba*);
    ~DictProducer();

    void buildEnDict();
    void buildCnDict();
    void buildIndex();
    void store();
    size_t getByteNum_UTF8(const char byte);

    //调试函数
    void showFile();

private:
    vector<string> _file;
    vector<pair<string,int>> _dict;
    map<string,set<int>> _index;
    SplitEn* _cuttorEn;
    SplitZh* _cuttorZh;
    Configuration* _pConf;

};



#endif
