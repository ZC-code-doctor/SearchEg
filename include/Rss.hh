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
    string url;
};

class RssReader {
public:
    RssReader() = default;
    void parseRss(const string &filename);  // 解析RSS文件
    string dump();      // 输出到文件
    vector<RssItem> getItem();

private:
    vector<RssItem> _rss;
};

