#include "PageLibPreprocessor.h"
#include "WebPage.h"
#include "Configuration.h"
#include "Rss.hh"
#include <fstream>

using std::ofstream;

PageLibPreprocessor::PageLibPreprocessor(Configuration *pConf)
{
    // 初始化网页库对象
    initPageLib(pConf);

    //保存数据处理结果
    store();
}
PageLibPreprocessor::~PageLibPreprocessor()
{
}

void PageLibPreprocessor::initPageLib(Configuration *pConf)
{
    // 创建simhash对象清洗重复网页
    Simhasher simhasher("/home/lzc/SearchEg/include/cppjieba/dict/jieba.dict.utf8",
                        "/home/lzc/SearchEg/include/cppjieba/dict/hmm_model.utf8",
                        "/home/lzc/SearchEg/include/cppjieba/dict/idf.utf8",
                        "/home/lzc/SearchEg/include/cppjieba/dict/stop_words.utf8");

    string dirtPath = pConf->ConfigMap()["PageLibary"];
    vector<string> filePath = pConf->getDirt(dirtPath);

    int docid = 0;

    // 将语料库中的所有网页清洗后导入到网页库中
    for (auto &_file : filePath)
    {
        RssReader reader;
        // 输入文件
        reader.parseRss(_file);
        // 输出文件
        vector<RssItem> test = reader.getItem();
        for (auto &item : test)
        {
            //判读是否是没有内容的文章
            if(item.content.empty()&&item.description.empty())
            {
                break;
            }
            // 判读文章是否相同
            uint64_t fileHash;
            if (!CompareTexts(item.content+item.description, fileHash, simhasher))
            {
                // 不相同则添加进网页库中，并保存对应的文章id和文章hash到filehash容器中
                _pageLib.emplace_back(item, ++docid);
                _testHash.emplace_back(fileHash);
            }
            // _pageLib.emplace_back(item, ++docid);
        }
    }

}

// 计算海明距离的函数
int HammingDistance(uint64_t hash1, uint64_t hash2)
{
    uint64_t x = hash1 ^ hash2; // 异或操作
    int dist = 0;
    while (x)
    {
        dist += x & 1; // 统计 1 的个数
        x >>= 1;
    }
    return dist;
}

// 计算文章相似度函数
bool PageLibPreprocessor::CompareTexts(const std::string &text1, uint64_t &fileHash, const Simhasher &simhasher)
{
    // 生成两个文本的 SimHash
    simhasher.make(text1, 5, fileHash); // 5 是关键词个数
    for (auto &hash : _testHash)
    {
        // 计算海明距离
        int dist = HammingDistance(fileHash, hash);
        // 设置阈值，判断是否相似（例如：海明距离小于等于 12 时认为相似）
        if(dist<=8)
        {
            return true;
        }
    }
    //如果都不相同，则返回false
    return false;
}

// 将去重后的网页库保存到文件，并生成一个offPageLib
void PageLibPreprocessor::store()
{
    //打开文件
    ofstream ofs_page("/home/lzc/SearchEg/data/ripepage.dat");
    for(auto&item:_pageLib)
    {
        ofs_page<<item.getDoc()<<"\n";
    }

    //关闭文件
    ofs_page.close();
}