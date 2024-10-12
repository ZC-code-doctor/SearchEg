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
    SplitZh(); 
    virtual ~SplitZh();
    virtual vector<string> cut(Configuration*);
    cppjieba::Jieba* getJieba();
private:
    cppjieba::Jieba* _jieba;
    
};

#endif
