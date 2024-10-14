#include "SplitEn.h"
#include "Configuration.h"
#include <dirent.h>
#include <cstring>

using std::cout;

//构造函数
SplitEn::SplitEn(Configuration* pConf)
:_pConf(pConf)
{
}

//析构函数
SplitEn::~SplitEn()
{
    cout<<"~SplitEn\n";
}

//字符串切割操作
vector<string> SplitEn::cut(const string& filename)
{
    string dict_yuliao;
    string StopPath;
    vector<string> _file;
    //TODO:查找en的语料目录
    dict_yuliao = _pConf->ConfigMap()[filename];

    //查找该目录下的语料文件
    //查找英文语料库
    _file = _pConf->getDirt(dict_yuliao); 

    //加载停用词
    set<string> StopWord = _pConf->getStopWordList();
    
    //打开文件
    vector<string> srcDict;
    srcDict.reserve(10000);
    
    for(const auto& dirPath:_file)
    {
        // std::cout<<dirPath<<"\n";
        ifstream ifs(dirPath);
        string line,word;
        //按行读取文件内容
        while(getline(ifs,line))
        {
            istringstream iss(line);
            while(iss>>word)
            {
                //对每个单词进行清洗
                SplitEn::clean(word);
                if(!word.empty() && !StopWord.count(word))
                {
                    srcDict.emplace_back(word);
                }
            }
        }
        ifs.close();
    }
    //所有单词分割完毕，返回一份拷贝
    return srcDict;

}


// void SplitEn::clean(string& word)
// {
//     string temp;
//     for(auto& ch:word)
//     {
//         if(std::isalpha(ch)||std::isalnum(ch))
//         {
//             if(isupper(ch))
//             {
//                 ch = tolower(ch);
//             }
//             temp += ch;
//         }
//     }
//     word = temp;
// }

void SplitEn::clean(string& word)
{
    string temp;
    for (auto& ch : word)
    {
        if (std::isalnum(ch))  // isalnum 检查是否为字母或数字
        {
            temp += std::tolower(ch);  // 直接转换为小写
        }
    }
    word = temp;  // 将处理后的字符串赋值回 word
}








