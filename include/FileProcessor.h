#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include <string>

using std::string;
    
class FileProcessor
{
public:
    FileProcessor(const string&);
    ~FileProcessor();
    string process(string fileName);
private:
    string _titleFeature;
};




#endif
