#ifndef __SPLITEN_H__
#define __SPLITEN_H__

#include "SplitTool.h"


#include <fstream>
#include <sstream>
#include <unordered_set>

using std::unordered_set;
using std::istringstream;
using std::ifstream;

class Configuration;

class SplitEn
:public SplitTool
{
public:
    SplitEn(Configuration*);
    ~SplitEn();
    virtual vector<string> cut(const string&) override;

private:
    void clean(string& word);

private:
    Configuration* _pConf;

};

#endif
