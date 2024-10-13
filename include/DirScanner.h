#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <vector>
#include <string>

using std::vector;
using std::string;
class Configuration;

class DirScanner
{
public:
    DirScanner(string dir,Configuration*);
    ~DirScanner();
    vector<string>& getFiles();

private:
    vector<string> _file;
};




#endif
