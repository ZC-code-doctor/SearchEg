#include "PageLibPreprocessor.h"
#include "WebPage.h"
#include "Configuration.h"
#include "Rss.hh"
#include <fstream>

using std::ofstream;

PageLibPreprocessor::PageLibPreprocessor(Configuration *pConf, cppjieba::Jieba *jieba)
{
    // 初始化网页库对象
    initPageLib(pConf);

    // 生成倒排索引库
    initInverLib(jieba);
    // 保存数据处理结果
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
            // 判读是否是没有内容的文章
            if (item.content.empty() && item.description.empty())
            {
                break;
            }
            // 判读文章是否相同
            uint64_t fileHash;
            if (!CompareTexts(item.content + item.description, fileHash, simhasher))
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
        if (dist <= 8)
        {
            return true;
        }
    }
    // 如果都不相同，则返回false
    return false;
}

// 将去重后的网页库保存到文件，并生成一个offPageLib
void PageLibPreprocessor::store()
{
    // 打开文件
    ofstream ofs_page("/home/lzc/SearchEg/data/ripepage.dat");
    ofstream ofs_off("/home/lzc/SearchEg/data/offset.dat");
    // 记录打开时文件指针的位置
    std::streampos pos_cur = ofs_page.tellp();

    for (auto &item : _pageLib)
    {
        // TODO：查看当前打开位置
        //  获取文件指针位置
        std::streampos pos_before = ofs_page.tellp();
        int cur = pos_before - pos_cur;

        // 保存文件内容
        ofs_page << item.getDoc() << "\n";

        // 查看保存后文件指针位置，计算偏移量length；
        std::streampos pos_after = ofs_page.tellp();
        int length = pos_after - pos_before;

        // 保存文件的偏移量以及对应的文件id
        ofs_off << item.getDocId() << "\t" << cur << "\t" << length << "\n";
    }

    // 关闭文件
    ofs_off.close();
    ofs_page.close();
}

// 计算词频 (TF)
map<string, double> PageLibPreprocessor::computeTF(const vector<string> &words)
{
    map<string, int> wordCount;
    int totalWords = words.size();

    // 统计每个词的出现次数
    for (const auto &word : words)
    {
        wordCount[word]++;
    }

    // 计算 TF 值
    map<string, double> tf;
    for (const auto &[word, count] : wordCount)
    {
        tf[word] = (double)count / totalWords; // 词频 = 该词出现次数 / 总词数
    }

    return tf;
}

// 计算逆文档频率 (IDF)
map<string, double> PageLibPreprocessor::computeIDF(const vector<vector<string>> &documents)
{
    map<string, int> docCount;
    int totalDocs = documents.size();

    // 统计每个词在多少个文档中出现
    for (const auto &doc : documents)
    {
        set<string> uniqueWords(doc.begin(), doc.end()); // 使用 set 去重
        for (const auto &word : uniqueWords)
        {
            docCount[word]++;
        }
    }

    // 计算 IDF
    map<string, double> idf;
    for (const auto &[word, count] : docCount)
    {
        idf[word] = log((double)totalDocs / (count + 1)); // +1 防止除零
    }

    return idf;
}

// 计算 TF-IDF
map<string, double> PageLibPreprocessor::computeTFIDF(const vector<string> &words, const map<string, double> &idf)
{
    map<string, double> tf = computeTF(words);
    map<string, double> tfidf;

    for (const auto &[word, tfValue] : tf)
    {
        tfidf[word] = tfValue * idf.at(word); // TF-IDF = TF * IDF
    }

    return tfidf;
}

void PageLibPreprocessor::initInverLib(cppjieba::Jieba *)
{
}