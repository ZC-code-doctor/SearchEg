#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

using namespace tinyxml2;
using namespace std;

struct RssItem {
    string title;
    string link;
    string description;
    string content;
};

class RssReader {
public:
    RssReader() = default;
    void parseRss(const string &filename);  // 解析RSS文件
    void dump(const string &filename);      // 输出到文件

private:
    vector<RssItem> _rss;
};

