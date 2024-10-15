#include "Tools.h"

// 计算词频 (TF)
map<string, double> computeTF(const vector<string> &words)
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
map<string, double> computeIDF(const vector<vector<string>> &documents)
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
map<string, double> computeTFIDF(const vector<string> &words, const map<string, double> &idf)
{
    map<string, double> tf = computeTF(words);
    map<string, double> tfidf;

    for (const auto &[word, tfValue] : tf)
    {
        tfidf[word] = tfValue * idf.at(word); // TF-IDF = TF * IDF
    }

    return tfidf;
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

vector<pair<int, double>> jaccardSimilarity(const vector<double> &vec1, const map<int, vector<double>> &vec2)
{
    size_t maxLength = std::max(vec1.size(), vec2.size());
    vector<pair<int, double>> res;
    vector<double> v1 = vec1;
    vector<double> v2;
    v1.resize(maxLength, 0.0); // 用0补全
    for (auto &elem : vec2)
    {
        v2 = elem.second;
        v2.resize(maxLength, 0.0); // 用0补全
        double intersection = 0.0, unionCount = 0.0;
        for (size_t i = 0; i < maxLength; ++i)
        {
            if (v1[i] != 0.0 || v2[i] != 0.0)
            {
                unionCount++; // 有任意非零元素即计入并集
                if (v1[i] != 0.0 && v2[i] != 0.0)
                {
                    intersection++; // 两个向量对应位置同时非零
                }
            }
        }
        if (unionCount == 0.0) // 防止除以0
        {
            break;
        }
        res.emplace_back(elem.first,intersection / unionCount);
    }

    return res;
}
