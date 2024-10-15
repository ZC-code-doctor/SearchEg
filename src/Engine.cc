#include "Engine.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using namespace cppjieba;

Engine_basic::~Engine_basic()
{
}

Engine::Engine(Configuration *pConf, shared_ptr<cppjieba::Jieba> jieba)
    : _pConf(pConf), _jieba(jieba), _yuliaoTable(new vector<pair<string, int>>()), _indexTable(new map<string, set<int>>()), _offsetTable(new unordered_map<int, pair<int, int>>()), _invertIndexTable(new unordered_map<string, set<pair<int, double>>>())
{
    // 预留一片内存空间
    _offsetTable->reserve(10000);
    _invertIndexTable->reserve(10000);
    _yuliaoTable->reserve(30000);

    // 加载资源
    loadResoure();
}
Engine::~Engine()
{
    if (_yuliaoTable != nullptr)
    {
        delete _yuliaoTable;
        _yuliaoTable = nullptr;
    }
    if (_indexTable != nullptr)
    {
        delete _indexTable;
        _indexTable = nullptr;
    }

    if (_invertIndexTable != nullptr)
    {
        delete _invertIndexTable;
        _invertIndexTable = nullptr;
    }
    if (_offsetTable != nullptr)
    {
        delete _offsetTable;
        _offsetTable = nullptr;
    }
    std::cout << "~Engine" << "\n";
}

// 加载资源
void Engine::loadResoure()
{
    // 打开资源文件
    string yuliaoPath = _pConf->ConfigMap()["Yuliao"];
    string indexPath = _pConf->ConfigMap()["Index"];
    string offPath = _pConf->ConfigMap()["Offset"];
    string invertPath = _pConf->ConfigMap()["Invent"];

    // 将磁盘中的索引文件加载到内存中
    ifstream ifs_off(offPath);
    ifstream ifs_inv(invertPath);
    ifstream ifs_yuliao(yuliaoPath);
    ifstream ifs_idx(indexPath);

    string lines;
    while (getline(ifs_yuliao, lines))
    {
        string word;
        int frequency;
        istringstream iss(lines);
        iss >> word >> frequency;
        // 直接构造一个键值对插入到表中
        _yuliaoTable->emplace_back(word, frequency);
    }

    lines.clear();
    while (getline(ifs_idx, lines))
    {
        string keyWord;
        int lineNumber;
        // map<string,set<int>>
        istringstream iss(lines);
        if (iss >> keyWord)
        {
            set<int> lineSet;
            while (iss >> lineNumber)
            {
                lineSet.emplace(lineNumber);
            }
            _indexTable->emplace(keyWord, lineSet);
        }
    }

    lines.clear();
    while (getline(ifs_off, lines))
    {
        string docid, start, lengh;
        istringstream iss(lines);
        iss >> docid >> start >> lengh;
        // 插入一个网页索引信息
        _offsetTable->emplace(std::stoi(docid), std::make_pair(std::stoi(start), std::stoi(lengh)));
    }

    lines.clear();
    while (getline(ifs_inv, lines))
    {
        istringstream iss(lines);
        string keyWord, docid;
        double weight;

        // 读取关键字
        if (iss >> keyWord)
        {
            // 创建一个临时集合来存储当前关键字的文档和权重对
            set<pair<int, double>> tempSet;

            // 继续读取 docid 和 Weight
            while (iss >> docid >> weight)
            {
                // 将 docid 和 weight 作为一对插入临时集合中
                tempSet.emplace(stoi(docid), weight);
            }

            // 将集合插入到反向索引表中
            _invertIndexTable->emplace(keyWord, tempSet);
        }
    }

    // 关闭资源文件
    ifs_yuliao.close();
    ifs_idx.close();
    ifs_inv.close();
    ifs_off.close();
}

// 推荐候选词
vector<string> Engine::recommendWord(const string &keyWord)
{
    vector<string> ch = spiltWord(keyWord);
    
    return ch;
}


vector<string> Engine::spiltWord(const string& keyWord)
{
    vector<string> word;
    vector<string> ch;
    _jieba->Cut(keyWord, word, true);

    for (auto &elem : word)
    {
        size_t charNums = elem.size() / getByteNum_UTF8(elem[0]); // 计算字符数
        for (size_t idx = 0, n = 0; n < charNums; ++n) // 遍历字符
        {
            size_t charLen = getByteNum_UTF8(elem[idx]); // 获取字符的字节长度
            string subWord = elem.substr(idx, charLen);  // 按字节数提取子串

            // 判断是否为标点符号（扩展判断是否为中文标点）
            bool containsPunct = std::any_of(subWord.begin(), subWord.end(), [](unsigned char c){ 
                return std::ispunct(c);  // 基本标点符号范围扩展
            });

            if(!containsPunct)  // 只保留非标点符号
            {
                ch.emplace_back(subWord);
            }

            idx += charLen; // 移动到下一个字符
        }
    }
    return ch;
}

// 匹配网页
WebPage Engine::SearchPage(const string &)
{
}

size_t Engine::getByteNum_UTF8(const char byte)
{
    int byteNum = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        if (byte & (1 << (7 - i)))
            ++byteNum;
        else
            break;
    }
    return byteNum == 0 ? 1 : byteNum;
}
