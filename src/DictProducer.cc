#include "DictProducer.h"
#include "SplitTool.h"

#include <dirent.h>
#include <cstring>

DictProducer::DictProducer(const string& conf, SplitTool* tool)
:_cuttor(tool)
,_conf(conf)
{
    buildEnDict();
    createIndex();
    store();

}

//析构函数
DictProducer::~DictProducer()
{
    std::cout<<"~DictProducer\n";
}


void DictProducer::buildEnDict()
{
    //创建一个哈希map来记录所有原始语料的词频
    unordered_map<string,int> Temp;
    Temp.reserve(5000);
    vector<string> Src = _cuttor->cut(_conf);
    for(auto&words:Src)
    {
        Temp[words]+=1;
    }
    //将原始语料加入到_dict中
    for(auto&elem:Temp)
    {
        _dict.emplace_back(elem.first,elem.second);
    }

}
void DictProducer::buildCnDict()
{

}
void DictProducer::createIndex()
{
    size_t idx = 0;
    for(;idx<_dict.size();++idx)
    {
        for(const auto&ch:_dict[idx].first)
        {
           _index[string(1,ch)].insert(idx); 
        }
    }


}
void DictProducer::store()
{
    //生成候选词文件
   ofstream ofs_yuliao("/home/lzc/SearchEg/data/en_yuliao.dat");
   ofstream ofs_enIdx("/home/lzc/SearchEg/data/en_Index.dat");
   //生成英文语料文件
   for(const auto&elem: _dict)
   {
       ofs_yuliao<<elem.first<<"  "<<elem.second<<"\n";
   }
   //生成英文的单词索引文件
   for(const auto&elem:_index)
   {
       ofs_enIdx<<elem.first<<" ";
       for(const auto&num :elem.second)
       {
           ofs_enIdx<<num<<" ";
       }
       ofs_enIdx<<"\n";
   }

   ofs_yuliao.close();
   ofs_enIdx.close();
}

void DictProducer::showFile()
{
    for(auto&elem:_file)
    {
        std::cout<<elem<<"\n";
    }
}




