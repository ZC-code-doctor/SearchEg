#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

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

class DictProducer
{
public:
    DictProducer(const string&,SplitTool*);
    ~DictProducer();

    void buildEnDict();
    void buildCnDict();
    void createIndex();
    void store();

    //调试函数
    void showFile();

private:
    vector<string> _file;
    vector<pair<string,int>> _dict;
    map<string,set<int>> _index;
    SplitTool* _cuttor;
    string _conf;

};



#endif
