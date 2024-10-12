#include "SplitZh.h"
#include "Configuration.h"


SplitZh::SplitZh()
{
    const char* const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";
    const char* const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
    const char* const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
    const char* const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

    //初始化结巴对象
   _jieba = new cppjieba::Jieba (DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH); 
    
}

SplitZh::~SplitZh()
{
    if(_jieba)
    {
        delete _jieba;
        _jieba = nullptr;
        std::cout<<"~SplitZh"<<"\n";
    }
}

vector<string> SplitZh::cut(Configuration* conf)
{
    string dict_yuliao;
    string StopPath;
    vector<string> _file;
    //TODO:查找en的语料目录
    dict_yuliao = conf->ConfigMap()["Yuliao"];

    //查找该目录下的语料文件
    //查找中文语料库
    dict_yuliao += "zh_yuliao/";
    _file = conf->getDirt(dict_yuliao); 

    //加载停用词
    set<string> StopWord = conf->getStopWordList();
    
    //打开文件
    vector<string> srcDict;
    srcDict.reserve(10000);

    for(const auto& dirPath:_file)
    {
        // std::cout<<dirPath<<"\n";
        ifstream ifs(dirPath);
        string line,word;
        //按行读取文件内容
        while(getline(ifs,line))
        {
            
            
            
            
            
            
            
            
            
        }
        ifs.close();
    }
    //所有单词分割完毕，返回一份拷贝
    return srcDict;
}
