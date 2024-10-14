#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <cstring>
#include <vector>
#include <iostream>

using std::vector;
using std::ifstream;
using std::istringstream;
using std::map;
using std::string;
using std::set;

class Configuration
{
public:
    static Configuration* getInstance(const string&);
    map<string,string>& ConfigMap();
    set<string>& getStopWordList();
    //打开文件目录
    vector<string> getDirt(const string&dirt);

private:
    Configuration(const string& filepath);
    ~Configuration();
    //删除拷贝、赋值
    Configuration(const Configuration&)=delete ;
    Configuration& operator=(const Configuration&)=delete ;
    static void destory();
private:
    //辅助函数
    
    //初始化配置文件内容
    void initConfigs(const string&);
    //初始化停用词词集
    void initStopWord(const string&);
    

private:
    string _configFilePath;
    static Configuration* _pConf;
    map<string,string> _configs;
    set<string> _stopWordList;
};


#endif
