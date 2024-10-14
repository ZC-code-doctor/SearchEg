#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <set>

using namespace std;

// 计算词频 (TF)
map<string, double> computeTF(const vector<string>& words) {
    map<string, int> wordCount;
    int totalWords = words.size();

    // 统计每个词的出现次数
    for (const auto& word : words) {
        wordCount[word]++;
    }

    // 计算 TF 值
    map<string, double> tf;
    for (const auto& [word, count] : wordCount) {
        tf[word] = (double)count / totalWords;  // 词频 = 该词出现次数 / 总词数
    }

    return tf;
}

// 计算逆文档频率 (IDF)
map<string, double> computeIDF(const vector<vector<string>>& documents) {
    map<string, int> docCount;
    int totalDocs = documents.size();

    // 统计每个词在多少个文档中出现
    for (const auto& doc : documents) {
        set<string> uniqueWords(doc.begin(), doc.end());  // 使用 set 去重
        for (const auto& word : uniqueWords) {
            docCount[word]++;
        }
    }

    // 计算 IDF
    map<string, double> idf;
    for (const auto& [word, count] : docCount) {
        idf[word] = log((double)totalDocs / (count + 1));  // +1 防止除零
    }

    return idf;
}

// 计算 TF-IDF
map<string, double> computeTFIDF(const vector<string>& words, const map<string, double>& idf) {
    map<string, double> tf = computeTF(words);
    map<string, double> tfidf;

    for (const auto& [word, tfValue] : tf) {
        tfidf[word] = tfValue * idf.at(word);  // TF-IDF = TF * IDF
    }

    return tfidf;
}

// 分词函数（简单按空格分词）
vector<string> split(const string& text) {
    stringstream ss(text);
    string word;
    vector<string> words;

    while (ss >> word) {
        words.push_back(word);
    }

    return words;
}

int main() {
    // 示例文档集
    vector<string> doc1 = split("今天 天气 很好");
    vector<string> doc2 = split("今天 天气 不好");
    vector<string> doc3 = split("昨天 天气 很好");

    vector<vector<string>> documents = {doc1, doc2, doc3};

    // 计算 IDF 值
    map<string, double> idf = computeIDF(documents);

    // 对每个文档计算 TF-IDF
    for (int i = 0; i < documents.size(); ++i) {
        cout << "Document " << i + 1 << " TF-IDF values:" << endl;
        map<string, double> tfidf = computeTFIDF(documents[i], idf);
        for (const auto& [word, value] : tfidf) {
            cout << word << ": " << value + 1 << endl;
        }
        cout << endl;
    }

    return 0;
}
