#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include <string>

using std::string;

struct RssItem;


class WebPage
{
public:
    WebPage(RssItem item,int);
    ~WebPage();
    int getDocId();
    string getDoc();
    string content();
private:
    string _doc;
    int _docid;
    string _docTitle;
    string _docUrl;
    string _docContent;
};


#endif