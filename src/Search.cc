#include "Search.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;

Search_base::~Search_base() {};

Search::Search(Configuration *pConf)
    : _pConf(pConf), _offsetTable(new unordered_map<int, pair<int, int>>()), _invertIndexTable(new unordered_map<string, set<pair<int, double>>>())
{
    // 预留一片内存空间
    _offsetTable->reserve(10000);
    _invertIndexTable->reserve(10000);
    // 加载资源文件
    loadResoure();
}

Search::~Search()
{
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
    std::cout << "~Search" << "\n";
}

void Search::SearchPage()
{
    // for (auto &elem : *_offsetTable)
    // {
    //     std::cout << "docid=" << elem.first << " start=" << elem.second.first << " length=" << elem.second.second << "\n";
    // }

    // 测试输出以验证数据
    // for (const auto &entry : *_invertIndexTable)
    // {
    //     std::cout << "KeyWord: " << entry.first << "\n";
    //     for (const auto &pair : entry.second)
    //     {
    //         std::cout << "    DocID: " << pair.first << ", Weight: " << pair.second << "\n";
    //     }
    // }
}

void Search::loadResoure()
{
    string offPath = _pConf->ConfigMap()["Offset"];
    string invertPath = _pConf->ConfigMap()["Invent"];

    // 将磁盘中的索引文件加载到内存中
    ifstream ifs_off(offPath);
    ifstream ifs_inv(invertPath);
    string lines;
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

    ifs_inv.close();
    ifs_off.close();
}