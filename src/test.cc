
#include "Engine.h"
#include<iostream>

using std::cout;

const char *const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";
const char *const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
const char *const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
const char *const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

int main()
{
    // 初始化结巴对象
    shared_ptr<cppjieba::Jieba>jieba(new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH));
    Configuration *pConf =  Configuration::getInstance("../conf/Myconf.conf");

    Engine rec(pConf,jieba);

    vector<string> temp = rec.recommendWord("Hello,世界");
    for(auto&word:temp)
    {
        std::cout<<word<<"\n";
    }
    

    return 0;
}