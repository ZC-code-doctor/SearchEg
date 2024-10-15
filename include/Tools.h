#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <map>
#include <string>
#include <vector>
#include <set>
#include <math.h>
#include <string>

using std::string;
using std::set;
using std::vector;
using std::string;
using std::map;
using std::pair;


// 计算词频 (TF)
map<string, double> computeTF(const vector<string> &words);
// 计算逆文档频率 (IDF)
map<string, double> computeIDF(const vector<vector<string>> &documents);
// 计算 TF-IDF
map<string, double> computeTFIDF(const vector<string> &words, const map<string, double> &idf);
// 计算海明距离的函数
int HammingDistance(uint64_t hash1, uint64_t hash2);
//Jaccard 相似度衡量两个集合相似度
map<double,int> jaccardSimilarity(const vector<double> &vec1, const map<int,vector<double>> &vec2);

//最小编辑算法
size_t nBytesCode(const char ch);
std::size_t length(const std::string &str);
int triple_min(const int &a, const int &b, const int &c);
int editDistance(const std::string &lhs, const std::string &rhs);

#endif