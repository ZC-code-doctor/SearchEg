#include "Tools.h"

#include <sstream>

using std::istringstream;

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

// 文件相似度比较
map<double, int> jaccardSimilarity(const vector<double> &vec1, const map<int, vector<double>> &vec2)
{
    size_t maxLength = std::max(vec1.size(), vec2.size());
    map<double, int> res;
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
        // res.emplace_back(elem.first,intersection / unionCount);
        res.emplace(intersection / unionCount, elem.first);
    }

    return res;
}

size_t nBytesCode(const char ch)
{
    if (ch & (1 << 7))
    {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx)
        {
            if (ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }
            else
                break;
        }
        return nBytes;
    }
    return 1;
}

std::size_t length(const std::string &str)
{
    std::size_t ilen = 0;
    for (std::size_t idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

int triple_min(const int &a, const int &b, const int &c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

// 最小编辑算法
int editDistance(const std::string &lhs, const std::string &rhs)
{
    // 计算最小编辑距离-包括处理中英文
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for (size_t idx = 0; idx <= lhs_len; ++idx)
    {
        editDist[idx][0] = idx;
    }
    for (size_t idx = 0; idx <= rhs_len; ++idx)
    {
        editDist[0][idx] = idx;
    }
    std::string sublhs, subrhs;
    for (std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i,
                     ++lhs_idx)
    {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);
        for (std::size_t dist_j = 1, rhs_idx = 0;
             dist_j <= rhs_len; ++dist_j, ++rhs_idx)
        {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            if (sublhs == subrhs)
            {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j -
                                                                1];
            }
            else
            {
                editDist[dist_i][dist_j] =
                    triple_min(editDist[dist_i][dist_j - 1] + 1,
                               editDist[dist_i - 1][dist_j] + 1,
                               editDist[dist_i - 1][dist_j - 1] + 1);
            }
        }
    }
    return editDist[lhs_len][rhs_len];
}

//解析HTTP请求
void HttpRequest::parseRequestLine(const std::string& line)
{
        std::istringstream stream(line);
        stream >> method;  // 提取请求方法
        stream >> url;     // 提取URL
}

void HttpRequest::addHeader(const string &headerLine)
{
    size_t pos = headerLine.find(':');
    if (pos != string::npos)
    {
        string key = headerLine.substr(0, pos);
        string value = headerLine.substr(pos + 1);
        headers[key] = value;
    }
}

void HttpRequest::setBody(const string &bodyContent)
{
    body = bodyContent;
}

HttpRequest parseHttpRequest(const std::string &msg)
{
    HttpRequest request;
    std::istringstream ss(msg);
    std::string line;

    // 解析请求行 (第一行)
    if (std::getline(ss, line))
    {
        request.parseRequestLine(line);
    }

    // 解析请求头
    while (std::getline(ss, line) && line != "\r")
    {
        if (!line.empty())
        {
            request.addHeader(line);
        }
    }

    // 判断是否有请求体
    // auto it = request.headers.find("Content-Length");
    // if (it != request.headers.end())
    // {
    //     int contentLength = std::stoi(it->second);
    //     if (contentLength > 0)
    //     {
    //         // 读取请求体
    //         std::string body;
    //         char ch;
    //         for (int i = 0; i < contentLength; ++i)
    //         {
    //             ss.get(ch);
    //             body.push_back(ch);
    //         }
    //         request.setBody(body);  // 设置请求体
    //     }
    // }

    return request;
}

// 函数用于解码 URL 中的百分号编码
std::string urlDecode(const std::string& src) {
    std::string decoded;
    for (size_t i = 0; i < src.length(); ++i) {
        if (src[i] == '%' && i + 2 < src.length() &&
            std::isxdigit(src[i + 1]) && std::isxdigit(src[i + 2])) {
            int value = std::stoi(src.substr(i + 1, 2), nullptr, 16);
            decoded += static_cast<char>(value);
            i += 2; // 跳过已处理的两个字符
        } else {
            decoded += src[i];
        }
    }
    return decoded;
}

std::string parseUrl(const std::string& url) {
    std::string result;

    // 检查是否以 "/search" 开头
    if (url.find("/search") == 0) {
        // 获取关键字部分
        std::string queryString = url.substr(url.find("?") + 1);

        // 使用字符串流解析关键字
        std::istringstream iss(queryString);
        std::string token;
        std::string keyword;

        // 按照 '=' 分割，获取关键字
        while (std::getline(iss, token, '=')) {
            if (token == "keyword") {
                std::getline(iss, keyword);
                break;
            }
        }

        // 解码关键字
        keyword = urlDecode(keyword);

        // 构造结果字符串
        result = keyword;
    } else {
        result = "无效的指令";
    }

    return result;
}