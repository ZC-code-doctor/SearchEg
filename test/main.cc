#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

// 解析 HTTP 请求行
struct HttpRequest {
    string method; // 请求方法
    string url;    // 请求的 URL
    string version; // HTTP 版本
    map<string, string> headers; // 请求头
    string body;   // 请求体 (如果有)

    void parseRequestLine(const string& requestLine) {
        istringstream iss(requestLine);
        iss >> method >> url >> version;
    }

    void addHeader(const string& headerLine) {
        size_t pos = headerLine.find(':');
        if (pos != string::npos) {
            string key = headerLine.substr(0, pos);
            string value = headerLine.substr(pos + 1);
            headers[key] = value;
        }
    }

    void setBody(const string& bodyContent) {
        body = bodyContent;
    }
};

// 解析收到的 HTTP 请求
HttpRequest parseHttpRequest(const string& msg) {
    HttpRequest request;
    istringstream ss(msg);
    string line;

    // 解析请求行 (第一行)
    if (getline(ss, line)) {
        request.parseRequestLine(line);
    }

    // 解析请求头
    while (getline(ss, line) && line != "\r") {
        if (!line.empty()) {
            request.addHeader(line);
        }
    }

    // 解析请求体 (可选)
    if (getline(ss, line)) {
        request.setBody(line);
    }

    return request;
}

int main() {
    string httpMsg = "GET /index.html HTTP/1.1\r\n"
                     "Host: www.example.com\r\n"
                     "User-Agent: Mozilla/5.0\r\n"
                     "Accept: text/html\r\n\r\n";

    HttpRequest request = parseHttpRequest(httpMsg);

    // 输出解析结果
    cout << "Method: " << request.method << endl;
    cout << "URL: " << request.url << endl;
    cout << "Version: " << request.version << endl;

    for (const auto& header : request.headers) {
        cout << "Header: " << header.first << " = " << header.second << endl;
    }

    if (!request.body.empty()) {
        cout << "Body: " << request.body << endl;
    }

    return 0;
}

