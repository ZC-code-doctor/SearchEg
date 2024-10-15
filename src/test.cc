
#include "Engine.h"
#include "Tools.h"

#include <iostream>
#include <cstdlib>

using std::cout;

const char *const DICT_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/jieba.dict.utf8";
const char *const HMM_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/user.dict.utf8";
const char *const IDF_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/idf.utf8";
const char *const STOP_WORD_PATH = "/home/lzc/SearchEg/include/cppjieba/dict/stop_words.utf8";

int main()
{
    // 初始化结巴对象
    shared_ptr<cppjieba::Jieba> jieba(new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH));
    Configuration *pConf = Configuration::getInstance("/home/lzc/SearchEg/conf/Myconf.conf");

    Engine rec(pConf, jieba);
    while (1)
    {
        string words;
        std::cout<<"输入关键词: ";
        std::cin >> words;
        // rec.SearchPage("景德镇非遗陶瓷色釉堆雕技艺代表性传承人");
        // vector<json> res = rec.SearchPage(words);

        // for (const auto &item : res)
        // {
        //     std::cout << item.dump(4) << std::endl; // 4 是缩进格式
        // }
        vector<string> res = rec.recommendWord(words);
        std::cout<<"候选词：";
        for(auto&elem:res)
        {
            std::cout<<elem<<",";
        }
        std::cout<<"\n";
    }

    return 0;
}