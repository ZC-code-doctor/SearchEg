#include "SplitZh.h"
#include "Configuration.h"
#include "cppjieba/Jieba.hpp"


SplitZh::SplitZh(cppjieba::Jieba* jieba,Configuration* conf)
:_jieba(jieba)
,_conf(conf)
{

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

vector<string> SplitZh::cut(const string& filename)
{
    string dict_yuliao;
    string StopPath;
    vector<string> _file;
    //TODO:查找en的语料目录
    dict_yuliao = _conf->ConfigMap()[filename];

    //查找该目录下的语料文件
    //查找中文语料库
    _file = _conf->getDirt(dict_yuliao);

    // //加载停用词
    set<string> StopWord = _conf->getStopWordList();
    
    //打开文件
    vector<string> srcDict;
    srcDict.reserve(10000);

    for(const auto& dirPath:_file)
    {
        // std::cout<<dirPath<<"\n";
        ifstream ifs(dirPath);
        string line;
        vector<string> words;
        //按行读取文件内容
        while(getline(ifs,line))
        {
           _jieba->Cut(line,words,true); 
           for(auto& word:words)
           {
                bool containsPunct = std::any_of(word.begin(), word.end(), [](char c){ return std::ispunct(static_cast<unsigned char>(c)); });
                if(!word.empty() && word!=" "&& !containsPunct && !StopWord.count(word) )
                {
                    srcDict.push_back(word);
                }
           }
            
        }
        ifs.close();
    }
    
    //所有单词分割完毕，返回一份拷贝
    return srcDict;
}

