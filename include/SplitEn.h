#ifndef __SPLITEN_H__
#define __SPLITEN_H__

#include "SplitTool.h"


#include <fstream>
#include <sstream>
#include <unordered_set>

using std::unordered_set;
using std::istringstream;
using std::ifstream;

class SplitEn
:public SplitTool
{
public:
    SplitEn();
    ~SplitEn();
    virtual vector<string> cut(const string& path) override;


private:
    unordered_set<string> load_StopWord(const string&);
    void clean(string& word);

private:

};

#endif
