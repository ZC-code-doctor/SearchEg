#ifndef __SPLITZH_H__
#define __SPLITZH_H__

#include "SplitTool.h"
#include "cppjieba/Jieba.hpp"

#include <vector>
#include <string>

using std::vector;
using std::string;

class SplitZh
:public SplitTool
{
public:
    SplitZh(cppjieba::Jieba*,Configuration* conf); 
    virtual ~SplitZh();
    virtual vector<string> cut(const string& filename);
private:
    cppjieba::Jieba* _jieba;
    Configuration* _conf;
    
};

#endif
