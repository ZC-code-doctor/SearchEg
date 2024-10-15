#include "Rss.hh"

//解析Rss文件
void RssReader::parseRss(const string &filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        cerr << "Error loading file: " << filename << endl;
        return;
    }

    XMLElement *root = doc.FirstChildElement("rss");
    if (!root) {
        cerr << "Invalid RSS feed" << endl;
        return;
    }

    XMLElement *channel = root->FirstChildElement("channel");
    if (!channel) {
        cerr << "No channel found in RSS feed" << endl;
        return;
    }

    RssItem rssItem;

    XMLElement *imageElement = channel->FirstChildElement("image");
    if (imageElement) {
    XMLElement *urlElement = imageElement->FirstChildElement("url");
    if (urlElement && urlElement->GetText()) {
        rssItem.url = urlElement->GetText();
    }
    }

    XMLElement *item = channel->FirstChildElement("item");
    while (item) {

        XMLElement *titleElement = item->FirstChildElement("title");
        if (titleElement && titleElement->GetText()) {
            rssItem.title = titleElement->GetText();
        }

        XMLElement *linkElement = item->FirstChildElement("link");
        if (linkElement && linkElement->GetText()) {
            rssItem.link = linkElement->GetText();
        }

        XMLElement *descElement = item->FirstChildElement("description");
        if (descElement && descElement->GetText()) {
            string description = descElement->GetText();
            std::regex htmlTags("<[^>]+>|&[^;]+;");
            rssItem.description = regex_replace(description, htmlTags, "");
        }

        XMLElement *contentElement = item->FirstChildElement("content:encoded");
        if (contentElement && contentElement->GetText()) {
            // 使用正则表达式去除 HTML 标签
            string content = contentElement->GetText(); 
            std::regex htmlTags("<[^>]+>|&[^;]+;");
            rssItem.content = regex_replace(content, htmlTags, "");
        }

        _rss.push_back(rssItem);
        item = item->NextSiblingElement("item");
    }
}


//导出数据到指定文件
string RssReader::dump() {

    static int docid = 0;
    ++docid;

    string res;
    for (const auto &rssItem : _rss) {

        res = "<doc><docid>" + to_string(docid) + "</docid><url>" + rssItem.url + "</url><title>" + rssItem.title;
        if(!rssItem.content.empty())
        {
            res += "</title><content>" + rssItem.content + "</content></doc>";
        }
        else if(!rssItem.description.empty())
        {
            res += "</title><description>" + rssItem.description + "</description></doc>";
        }
        else if(rssItem.content.empty()&&rssItem.description.empty())
        {
            --docid;
            return string("") ;
        }
    }
    return res;
}

vector<RssItem> RssReader::getItem()
{
    return _rss;
}