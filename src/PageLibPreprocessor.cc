#include "PageLibPreprocessor.h"
#include "WebPage.h"
#include "Configuration.h"
#include "Rss.hh"
#include "Tools.h"
#include <fstream>

using std::ofstream;

PageLibPreprocessor::PageLibPreprocessor(Configuration *pConf, cppjieba::Jieba *jieba)
: _jieba(jieba)
{
    // 初始化网页库对象
    initPageLib(pConf);

    // 生成倒排索引库
    initInverLib();

    // 保存数据处理结果
    store();
}
PageLibPreprocessor::~PageLibPreprocessor()
{
}

//创建去重网页库
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
    ofstream ofs_inv("/home/lzc/SearchEg/data/invent.dat");

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

    //将倒排索引输入到文件中
    for(auto&invent:_invertIndexTable)
    {
        ofs_inv<<invent.first<<"\t";
        for(auto&Weight:invent.second)
        {
            ofs_inv<<Weight.first<<" "<<Weight.second<<" ";
        }
        ofs_inv<<"\n";
    }

    // 关闭文件
    ofs_off.close();
    ofs_page.close();
    ofs_inv.close();
}

// 切割文章
vector<string> PageLibPreprocessor::split(const string &text)
{
    vector<string> words;
    _jieba->Cut(text,words,true);
    return words;
}

//建立倒排索引
void PageLibPreprocessor::initInverLib()
{
    //预留10000字节的空间
    vector<vector<string>> documents;
    documents.reserve(10000);
    for(auto&webPage:_pageLib)
    {
        vector<string> TF_Word = split(webPage.content());
        documents.push_back(TF_Word);
    }
    map<string, double> idf = computeIDF(documents);
    // 对每个文档计算 TF-IDF
    for (int i = 0; i < documents.size(); ++i) {
        map<string, double> tfidf = computeTFIDF(documents[i], idf);
        for (const auto& [word, value] : tfidf) {

            _invertIndexTable[word].insert(pair<int,double>(i+1,value));
        }
    }
}