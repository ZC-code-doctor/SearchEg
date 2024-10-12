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

    XMLElement *item = channel->FirstChildElement("item");
    while (item) {
        RssItem rssItem;

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
            rssItem.description = descElement->GetText();
        }

        XMLElement *contentElement = item->FirstChildElement("content:encoded");
        if (contentElement && contentElement->GetText()) {
            // 使用正则表达式去除 HTML 标签
            string content = contentElement->GetText();
            regex htmlTags("<[^>]*>");
            rssItem.content = regex_replace(content, htmlTags, "");
        }

        _rss.push_back(rssItem);
        item = item->NextSiblingElement("item");
    }
}


//导出数据到指定文件
void RssReader::dump(const string &filename) {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "Error opening output file: " << filename << endl;
        return;
    }

    int docid = 1;
    for (const auto &rssItem : _rss) {
        ofs << "<doc>" << endl;
        ofs << "\t<docid>" << docid++ << "</docid>" << endl;
        ofs << "\t<title>" << rssItem.title << "</title>" << endl;
        ofs << "\t<link>" << rssItem.link << "</link>" << endl;
        ofs << "\t<description>" << rssItem.description << "</description>" << endl;
        ofs << "\t<content>" << rssItem.content << "</content>" << endl;
        ofs << "</doc>" << endl;
    }
}

