#include "PageLibPreprocessor.h"
#include "WebPage.h"
#include "Configuration.h"
#include "Rss.hh"


PageLibPreprocessor::PageLibPreprocessor(Configuration *pConf)
{
    // 初始化网页库对象
    initPageLib(pConf);
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
            bool isSimilar = false; // 用于标记是否找到相似文档
            // 每一个文件都需要和之前的文件进行对比，相同则不加入
            for (auto &existingDoc : _pageLib)
            {
                if (CompareTexts(existingDoc.getDoc(), item.content, simhasher))
                {
                    isSimilar = true; // 找到相似文档
                    break;            // 跳出内层循环
                }
            }
            // 如果没有找到相似文档，才将当前 item 加入 _pageLib
            if (!isSimilar)
            {
                _pageLib.emplace_back(item, ++docid);
            }
        }
    }
}

// 计算海明距离的函数
int PageLibPreprocessor::HammingDistance(uint64_t hash1, uint64_t hash2)
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

bool PageLibPreprocessor::CompareTexts(const std::string &text1, const std::string &text2, const Simhasher &simhasher)
{
    uint64_t hash1, hash2;

    // 生成两个文本的 SimHash
    simhasher.make(text1, 5, hash1); // 5 是关键词个数
    simhasher.make(text2, 5, hash2);

    // 计算海明距离
    int dist = HammingDistance(hash1, hash2);

    std::cout << "Hamming Distance: " << dist << std::endl;

    // 设置阈值，判断是否相似（例如：海明距离小于等于 3 时认为相似）
    if (dist <= 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}