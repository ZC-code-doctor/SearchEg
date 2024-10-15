
#include "SplitTool.h"
#include "SplitEn.h"
#include "DictProducer.h"
#include "Configuration.h"
#include "SplitZh.h"
#include "PageLibPreprocessor.h"
#include "Configuration.h"

#include <iostream>

const char *const DICT_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/jieba.dict.utf8";
const char *const HMM_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/user.dict.utf8";
const char *const IDF_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/idf.utf8";
const char *const STOP_WORD_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/stop_words.utf8";


int main()
{

    // 初始化结巴对象
    cppjieba::Jieba *jieba = new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
    Configuration *pConf =  Configuration::getInstance("/home/lzc/SearchEg/conf/Myconf.conf");

    //初始化词典资源
    DictProducer temp(pConf,jieba);
    //初始化网页库资源
    PageLibPreprocessor PageLib(pConf, jieba);

    return 0;
}