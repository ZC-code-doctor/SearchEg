#include "SplitEn.h"
#include <dirent.h>
#include <cstring>

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
vector<string> SplitEn::cut(const string& conf)
{
    string dict_yuliao;
    string StopPath;
    vector<string> _file;
    //TODO:查找en的语料目录
    ifstream ifs(conf);
    string line,words;
    while(getline(ifs,line))
    {
        istringstream iss(line);
        while(iss>>words)
        {
            if(words == "Yuliao")
            {
                iss>>words;
                dict_yuliao = words;
                break;
            }
        }
    }

    //查找该目录下的语料文件
    //查找英文语料库
    StopPath = dict_yuliao + "en_stop/stop_words_eng.txt";
    dict_yuliao += "en_yuliao";
    DIR* dir = opendir(dict_yuliao.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening directory" << std::endl;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // 跳过特殊目录项 "." 和 ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            string res = dict_yuliao +"/" +string(entry->d_name);
            _file.push_back(res);   //保存文件名到_file
        }
    }
    //加载停用词
    unordered_set<string> StopWord = load_StopWord(StopPath);
    
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
    }
    //所有单词分割完毕，返回一份拷贝
    return srcDict;

}


void SplitEn::clean(string& word)
{
    string temp;
    for(auto& ch:word)
    {
        if(std::isalpha(ch)||std::isalnum(ch))
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
unordered_set<string> SplitEn::load_StopWord(const string& path)
{
   ifstream ifs(path);
   string word;
   unordered_set<string> temp; 
   temp.reserve(1000);
   while (getline(ifs, word)) {
       // 清除单词前后的空白字符
       word.erase(0, word.find_first_not_of(" \t\n\r"));
       word.erase(word.find_last_not_of(" \t\n\r") + 1);
       temp.insert(word);
   }
   
   return temp;
}
