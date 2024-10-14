#include "SplitTool.h"
#include "SplitEn.h"
#include "DictProducer.h"
#include "Configuration.h"
#include "SplitZh.h"

#include <iostream>

int main()
{
    Configuration *pConf = Configuration::getInstance("../conf/Myconf.conf");
    const char *const DICT_PATH = "../include/cppjieba/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "../include/cppjieba/dict/hmm_model.utf8";
    const char *const USER_DICT_PATH = "../include/cppjieba/dict/user.dict.utf8";
    const char *const IDF_PATH = "../include/cppjieba/dict/idf.utf8";
    const char *const STOP_WORD_PATH = "../include/cppjieba/dict/stop_words.utf8";

    // 初始化结巴对象
    cppjieba::Jieba* jieba = new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);
    // 预热阶段,加载关键字、网页资源
    {
        DictProducer temp(pConf,jieba);
    }
    // delete jieba;

    return 0;
}
