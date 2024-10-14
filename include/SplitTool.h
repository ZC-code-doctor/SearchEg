#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include <vector>
#include <string>
#include <iostream>

using std::vector;
using std::string;

class Configuration;

class SplitTool
{
public:
    virtual ~SplitTool()=0;
    virtual vector<string> cut(const string&)=0;
private:

};




#endif
