#include "WebPage.h"
#include "Rss.hh"

WebPage::WebPage(RssItem item,int id)
{
    _docid = id;
    _docTitle = item.title;
    _docUrl = item.link;
    if(item.content.empty())
    {
        _docContent = item.description;
    }
    else
    {
        _docContent = item.content;
    }

    _doc = "<doc><docid>" + to_string(_docid) + "</docid><url>" + _docUrl + "</url><title>" + _docTitle + 
            "</title><content>" + _docContent + "</content></doc>";
}
WebPage::~WebPage()
{

}
int WebPage::getDocId()
{
    return _docid;
}
string WebPage::getDoc()
{
    return _doc;
}
string WebPage::content()
{
    return _docContent;
}