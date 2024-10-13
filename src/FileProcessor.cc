#include "FileProcessor.h"
#include "Rss.hh"

FileProcessor::FileProcessor(const string& title)
:_titleFeature(title)
{

}

FileProcessor::~FileProcessor()
{
    
}

string FileProcessor::process(string fileName)
{
    RssReader reader;
    //输入文件
    reader.parseRss(fileName);
    //输出文件
    return reader.dump();
}






