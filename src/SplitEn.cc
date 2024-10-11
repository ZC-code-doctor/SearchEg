#include "SplitEn.h"

using std::cout;

//构造函数
SplitEn::SplitEn()
{
}

//析构函数
SplitEn::~SplitEn()
{
    cout<<"~SplitEn\n";
}

//字符串切割操作
vector<string> SplitEn::cut(const string& srcPath)
{
    string rootPath = "/home/lzc/SearchEg/yuliao/";
    string dirPath = rootPath + srcPath;

    //打开文件
    ifstream ifs(dirPath);
    string line,word;
    vector<string> srcDict;
    srcDict.reserve(10000);
    //按行读取文件内容
    while(getline(ifs,line))
    {
       istringstream iss(line);
       while(iss>>word)
       {
           //对每个单词进行清洗
           SplitEn::clean(word);
           if(!word.empty())
           {
             srcDict.emplace_back(word);
           }
       }
    }

    //所有单词分割完毕，返回一份拷贝
    return srcDict;

}


void SplitEn::clean(string& word)
{
    string temp;
    for(auto& ch:word)
    {
        if(std::isalpha(ch))
        {
            if(isupper(ch))
            {
                ch = tolower(ch);
            }
            temp += ch;
        }
    }
    word = temp;
}
