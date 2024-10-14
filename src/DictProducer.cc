#include "DictProducer.h"
#include "SplitTool.h"
#include "SplitEn.h"
#include "Configuration.h"

#include <dirent.h>
#include <cstring>

DictProducer::DictProducer(Configuration *pConf, cppjieba::Jieba *jieba)
: _pConf(pConf)
,_cuttorEn(new SplitEn(pConf))
,_cuttorZh(new SplitZh(jieba, pConf))
{
    // 初始化词法切割器

    buildEnDict();
    buildCnDict();
    buildIndex();
    store();
}

// 析构函数
DictProducer::~DictProducer()
{
    if(_cuttorEn!=nullptr)
    {
        delete _cuttorEn;
        _cuttorEn = nullptr;
    }
    if(_cuttorZh!=nullptr)
    {
        delete _cuttorZh;
        _cuttorZh = nullptr;
    }
    std::cout << "~DictProducer\n";
}

void DictProducer::buildEnDict()
{
    // 创建一个哈希map来记录所有原始语料的词频
    unordered_map<string, int> Temp;
    Temp.reserve(5000);
    vector<string> Src = _cuttorEn->cut("Yuliao_En");
    for (auto &words : Src)
    {
        Temp[words] += 1;
    }
    // 将原始语料加入到_dict中
    for (auto &elem : Temp)
    {
        _dict.emplace_back(elem.first, elem.second);
    }
}
void DictProducer::buildCnDict()
{
    // 创建一个哈希map来记录所有原始语料的词频
    unordered_map<string, int> Temp;
    Temp.reserve(5000);
    vector<string> Src = _cuttorZh->cut("Yuliao_Zh");
    for (auto &words : Src)
    {
        Temp[words] += 1;
    }
    // 将原始语料加入到_dict中
    for (auto &elem : Temp)
    {
        _dict.emplace_back(elem.first, elem.second);
    }
}

void DictProducer::buildIndex()
{
    int i = 0; // 记录下标
    for (auto elem : _dict)
    {
        string word = elem.first;
        size_t charNums = word.size() / getByteNum_UTF8(word[0]);
        for (size_t idx = 0, n = 0; n != charNums; ++idx, ++n) // 得到字符数
        {
            // 按照字符个数切割
            size_t charLen = getByteNum_UTF8(word[idx]);
            string subWord = word.substr(idx, charLen); // 按照编码格式，进行拆分
            _index[subWord].insert(i);
            idx += (charLen - 1);
        }
        ++i;
    }
}

void DictProducer::store()
{
    // 生成候选词文件
    ofstream ofs_yuliao("/home/lzc/SearchEg/data/yuliao.dat");
    ofstream ofs_enIdx("/home/lzc/SearchEg/data/Index.dat");
    // 生成英文语料文件
    for (const auto &elem : _dict)
    {
        ofs_yuliao << elem.first << "  " << elem.second << "\n";
    }
    // 生成英文的单词索引文件
    for (const auto &elem : _index)
    {
        ofs_enIdx << elem.first << " ";
        for (const auto &num : elem.second)
        {
            ofs_enIdx << num << " ";
        }
        ofs_enIdx << "\n";
    }

    ofs_yuliao.close();
    ofs_enIdx.close();
}

void DictProducer::showFile()
{
    for (auto &elem : _file)
    {
        std::cout << elem << "\n";
    }
}

size_t DictProducer::getByteNum_UTF8(const char byte)
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
