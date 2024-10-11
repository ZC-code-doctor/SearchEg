#include "DictProducer.h"
#include "SplitTool.h"

#include <dirent.h>
#include <cstring>

DictProducer::DictProducer(const string& dirt , SplitTool* tool)
:_cuttor(tool)
{
    DIR* dir = opendir(dirt.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening directory" << std::endl;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // 跳过特殊目录项 "." 和 ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            _file.emplace_back(entry->d_name);  // 保存文件名到 _file
        }
    }

    closedir(dir);
}

//析构函数
DictProducer::~DictProducer()
{
    std::cout<<"~DictProducer\n";
}


void DictProducer::buildEnDict()
{
    for(auto& path:_file)
    {
        _cuttor->cut(path);
    }
    

}
void DictProducer::buildCnDict()
{

}
void DictProducer::createIndex()
{

}
void DictProducer::store()
{

}

void DictProducer::showFile()
{
    for(auto&elem:_file)
    {
        std::cout<<elem<<"\n";
    }
}




