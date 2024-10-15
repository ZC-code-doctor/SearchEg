#include "Configuration.h"

Configuration* Configuration::_pConf = nullptr;

Configuration* Configuration::getInstance(const string& confPath)
{
    if(_pConf==nullptr)
    {
        _pConf = new Configuration(confPath);
        atexit(Configuration::destory);
    }
    return _pConf;
}


map<string,string>& Configuration::ConfigMap()
{
    return _configs;
}


set<string>& Configuration::getStopWordList()
{
    return _stopWordList;
}

Configuration::Configuration(const string& confpath)
:_configFilePath(confpath)
{
    //初始化配置文件内容
    initConfigs(confpath);

    //获取停用词目录
    string StopDirt = _configs["StopDirt"];
    //调试接口
    // std::cout<<"获取到StopDirt: "<<StopDirt<<"\n";
    vector<string> StopWord_Path = getDirt(StopDirt);
    for(const auto& path:StopWord_Path)
    {
        // std::cout<<"path:"<<path<<"\n";
        initStopWord(path);
    }
    
    // for(auto& elem:_stopWordList)
    // {
    //     std::cout<<elem<<"\n";
    // }
    

}

Configuration::~Configuration()
{
    std::cout<<"~Configuration"<<"\n";
}
    
//初始化配置文件内容
void Configuration::initConfigs(const string& ConfPath)
{
    ifstream ifs(ConfPath);
    string line;
    //初始化配置文件内容的map
    while(getline(ifs,line))
    {
        istringstream iss(line);
        //每一行对应文件名 以及路径
        string filename,filepath;
        while(iss>>filename>>filepath)
        {
            //filename--->filepath
            _configs[filename] = filepath;    
        }
    }
    ifs.close();
}



//初始化停用词词集
void Configuration::initStopWord(const string& file)
{
    
    ifstream ifs(file);
    string word;
    while (getline(ifs, word)) {
        // 清除单词前后的空白字符
        word.erase(0, word.find_first_not_of(" \t\n\r"));
        word.erase(word.find_last_not_of(" \t\n\r") + 1);
        _stopWordList.insert(word);
    }
    ifs.close();
}


//打开文件目录
vector<string> Configuration::getDirt(const string&dirt)
{
    vector<string> filename;
    DIR* dir = opendir(dirt.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening directory" << std::endl;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // 跳过特殊目录项 "." 和 ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            //目录+文件名--->path / filename
            string res = dirt  + string(entry->d_name);
            filename.push_back(res);   //保存文件名到_file
        }
    }
    closedir(dir);
    return filename;
}

void Configuration::destory()
{
    if(_pConf!=nullptr)
    {
        delete _pConf;
        _pConf = nullptr;
        std::cout<<"destory"<<"\n";
    }
}





