#include "Tcp_server.h"
#include "Tcpconnection.h"
#include "ThreadPool.h"
#include "Task.h"
#include <iostream>

using std::cout;
using std::endl;

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

   Engine rec(pConf, jieba) ;
   std::cout<<"Engin size:"<<sizeof(Engine)<<"\n";
   Tcp_server server("192.168.88.128", 8888, &rec);

   server.Start();
}
