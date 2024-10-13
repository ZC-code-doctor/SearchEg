#include "DirScanner.h"
#include "Configuration.h"


DirScanner::DirScanner(string dir,Configuration* pConf)
{
    _file=pConf->getDirt(dir);    
}

DirScanner::~DirScanner()
{
    std::cout<<"~DirScanner\n";
}

vector<string>& DirScanner::getFiles()
{
    return _file;
}









