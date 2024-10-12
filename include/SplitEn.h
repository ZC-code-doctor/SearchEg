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
    SplitEn();
    ~SplitEn();
    virtual vector<string> cut(Configuration*) override;

private:
    void clean(string& word);

private:

};

#endif
