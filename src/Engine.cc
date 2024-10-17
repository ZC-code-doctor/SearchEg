#include "Engine.h"
#include "tinyxml2.h"

#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using namespace tinyxml2;
using namespace cppjieba;

Engine_basic::~Engine_basic()
{
}

Engine::Engine(Configuration *pConf, shared_ptr<cppjieba::Jieba> jieba)
    : _pConf(pConf)
    , _jieba(jieba)
    , _yuliaoTable(new vector<pair<string, int>>())
    , _indexTable(new map<string, set<int>>())
    , _offsetTable(new map<int, pair<int, int>>())
    , _invertIndexTable(new map<string, set<pair<int, double>>>())
    , _cacheManager()
{
    // 预留一片内存空间
    // _offsetTable->reserve(10000);
    // _invertIndexTable->reserve(10000);
    _yuliaoTable->reserve(30000);

    // 加载资源
    loadResoure();

    // 初始化主线的共享内存
    std::thread::id this_id = std::this_thread::get_id();
    std::cout<<"初始化主线程的id"<<this_id<<"\n";
    _cacheManager.getCache(this_id);
}
Engine::~Engine()
{
    if (_yuliaoTable != nullptr)
    {
        delete _yuliaoTable;
        _yuliaoTable = nullptr;
    }
    if (_indexTable != nullptr)
    {
        delete _indexTable;
        _indexTable = nullptr;
    }

    if (_invertIndexTable != nullptr)
    {
        delete _invertIndexTable;
        _invertIndexTable = nullptr;
    }
    if (_offsetTable != nullptr)
    {
        delete _offsetTable;
        _offsetTable = nullptr;
    }
    std::cout << "~Engine" << "\n";
}

// 加载资源
void Engine::loadResoure()
{
    // 打开资源文件
    string yuliaoPath = _pConf->ConfigMap()["Yuliao"];
    string indexPath = _pConf->ConfigMap()["Index"];
    string offPath = _pConf->ConfigMap()["Offset"];
    string invertPath = _pConf->ConfigMap()["Invent"];

    // 将磁盘中的索引文件加载到内存中
    ifstream ifs_off(offPath);
    ifstream ifs_inv(invertPath);
    ifstream ifs_yuliao(yuliaoPath);
    ifstream ifs_idx(indexPath);

    string lines;
    while (getline(ifs_yuliao, lines))
    {
        string word;
        int frequency;
        istringstream iss(lines);
        iss >> word >> frequency;
        // 直接构造一个键值对插入到表中
        _yuliaoTable->emplace_back(word, frequency);
    }

    lines.clear();
    while (getline(ifs_idx, lines))
    {
        string keyWord;
        int lineNumber;
        // map<string,set<int>>
        istringstream iss(lines);
        if (iss >> keyWord)
        {
            set<int> lineSet;
            while (iss >> lineNumber)
            {
                lineSet.emplace(lineNumber);
            }
            _indexTable->emplace(keyWord, lineSet);
        }
    }

    lines.clear();
    while (getline(ifs_off, lines))
    {
        string docid, start, lengh;
        istringstream iss(lines);
        iss >> docid >> start >> lengh;
        // 插入一个网页索引信息
        _offsetTable->emplace(std::stoi(docid), std::make_pair(std::stoi(start), std::stoi(lengh)));
    }

    lines.clear();
    while (getline(ifs_inv, lines))
    {
        istringstream iss(lines);
        string keyWord, docid;
        double weight;

        // 读取关键字
        if (iss >> keyWord)
        {
            // 创建一个临时集合来存储当前关键字的文档和权重对
            set<pair<int, double>> tempSet;

            // 继续读取 docid 和 Weight
            while (iss >> docid >> weight)
            {
                // 将 docid 和 weight 作为一对插入临时集合中
                tempSet.emplace(stoi(docid), weight);
            }

            // 将集合插入到反向索引表中
            _invertIndexTable->emplace(keyWord, tempSet);
        }
    }

    // 关闭资源文件
    ifs_yuliao.close();
    ifs_idx.close();
    ifs_inv.close();
    ifs_off.close();
}

// 推荐候选词
vector<string> Engine::recommendWord(const string &keyWord)
{
    vector<string> ch = spiltWord(keyWord);
    vector<string> recommend;
    set<int> dictSet;
    for (auto &elem : ch)
    {
        for (auto &num : (*_indexTable)[elem])
        {
            dictSet.insert(num);
        }
    }
    for (auto &idx : dictSet)
    {
        int distance = editDistance(keyWord, (*_yuliaoTable)[idx].first);
        if (distance <= 1)
        {
            recommend.push_back((*_yuliaoTable)[idx].first);
        }
    }
    return recommend;
}

vector<string> Engine::spiltWord(const string &keyWord)
{
    vector<string> word;
    vector<string> ch;
    _jieba->Cut(keyWord, word, true);

    for (auto &elem : word)
    {
        size_t charNums = elem.size() / getByteNum_UTF8(elem[0]); // 计算字符数
        for (size_t idx = 0, n = 0; n < charNums; ++n)            // 遍历字符
        {
            size_t charLen = getByteNum_UTF8(elem[idx]); // 获取字符的字节长度
            string subWord = elem.substr(idx, charLen);  // 按字节数提取子串

            // 判断是否为标点符号（扩展判断是否为中文标点）
            bool containsPunct = std::any_of(subWord.begin(), subWord.end(), [](unsigned char c)
                                             { return std::ispunct(c); });

            if (!containsPunct) // 只保留非标点符号
            {
                ch.emplace_back(subWord);
            }

            idx += charLen; // 移动到下一个字符
        }
    }
    return ch;
}

// 匹配网页
vector<json> Engine::SearchPage(const string &keyWord)
{
    vector<string> word;
    vector<json> jsonPage;
    //通过线程id来锁定唯一的缓存地址
    std::thread::id this_id = std::this_thread::get_id();
    std::cout << "Current thread ID: " << this_id << std::endl;
    LRUcache& _cache = (*_cacheManager.getCache(this_id));
    bool isHit = _cache.readCache(keyWord, jsonPage);
    if (isHit)
    {
        std::cout<<"引擎缓存命中\n";
        return jsonPage;
    }
    else
    {
        std::cout<<"引擎缓存未命中\n";
        _jieba->Cut(keyWord, word, true);

        // 清洗字符串
        vector<string> SearchWord = cleanKeyWord(word);

        // 计算TF-IDF值生成查询向量
        std::vector<double> queryVector = KeyWord_TFIDF(SearchWord);

        // 获取一个关键词重合度高于x的文件集合
        map<int, vector<double>> fileVector = get_FileVecotr(SearchWord, queryVector.size(), 0.5);

        // 计算集合之间的相似度
        map<double, int> fileSimply = jaccardSimilarity(queryVector, fileVector);

        // 获取文章内容集合
        vector<pair<string, string>> matchFile = getFile(fileSimply);

        // 返回构建好的JSON对象
        for (auto &elem : matchFile)
        {
            // 构造 JSON 对象
            json j;
            j["url"] = elem.first;
            j["title"] = elem.second;
            jsonPage.push_back(j);
        }
        //往缓存中插入数据
        _cache.addElement(keyWord,jsonPage);
    }
    return jsonPage;
}

// 清洗字符串
vector<string> Engine::cleanKeyWord(vector<string> &word)
{
    vector<string> SearchWord;
    for (auto &elem : word)
    {
        bool containsPunct = std::any_of(elem.begin(), elem.end(), [](unsigned char c)
                                         { return std::ispunct(c); });

        if (!containsPunct)
        {
            SearchWord.emplace_back(elem);
        }
    }
    return SearchWord;
}

// 获取一个关键词重合度高于x的文件集合
map<int, vector<double>> Engine::get_FileVecotr(const vector<string> &SearchWord, int query_size, double threshold)
{
    // 创建一个所有有关关键词文章的maps
    map<int, vector<double>> srcVector;
    // 获取所有出现过关键词的网页的集合
    vector<set<pair<int, double>>> docidWeight = getDocidSet(SearchWord);
    for (auto &elem : docidWeight)
    {
        for (auto &kv : elem)
        {
            srcVector[kv.first].push_back(kv.second);
        }
    }

    map<int, vector<double>> fileVector;
    for (auto &elem : srcVector)
    {
        // 如果该文章下的向量数量与查询向量数一致，则说明所有查询词都出现在该文章中
        if (elem.second.size() >= query_size * threshold)
        {
            // 添加到文章特征向量容器中
            fileVector.emplace(elem.first, elem.second);
        }
    }
    return fileVector;
}

// 计算关键字的查询向量
vector<double> Engine::KeyWord_TFIDF(const vector<string> &SearchWord)
{
    // 利用TF-IDF算法算出每一个查询词的权重；
    // 计算TF值
    map<string, double> tf = computeTF(SearchWord);
    // 获取总文档数
    size_t N = _offsetTable->size();
    // 计算IDF值
    std::map<std::string, double> idf;
    for (const auto &word : SearchWord)
    {
        auto it = _invertIndexTable->find(word);
        int docCount = (it != _invertIndexTable->end()) ? it->second.size() : 0; // 包含此词的文档数
        idf[word] = log(N / (1.0 + docCount));                                   // 计算IDF
    }
    // 计算TF-IDF值生成查询向量
    std::vector<double> queryVector;
    for (const auto &word : SearchWord)
    {
        double tfidf = tf[word] * idf[word]; // 计算TF-IDF
        queryVector.push_back(tfidf);        // 加入查询向量
    }
    return queryVector;
}

// 获取倒排索引出现的集合
vector<set<pair<int, double>>> Engine::getDocidSet(const vector<string> &keyWord)
{
    vector<set<pair<int, double>>> res;
    for (auto &elem : keyWord)
    {
        res.emplace_back((*_invertIndexTable)[elem]);
    }
    return res;
}

// 获取文章内容集合
vector<pair<string, string>> Engine::getFile(const map<double, int> &fileSimply)
{
    auto it = fileSimply.rbegin();
    vector<pair<int, int>> offFile;
    vector<string> filePage;
    vector<pair<string, string>> res;
    filePage.reserve(1000);
    // 创建一个文件打开对象
    string pagePath = _pConf->ConfigMap()["Ripepage"];
    // 二进制形式打开，保证原汁原味读取
    ifstream ifs(pagePath, std::ios_base::binary);

    for (; it != fileSimply.rend(); ++it)
    {
        offFile.push_back((*_offsetTable)[(*it).second]);
    }
    for (auto &elem : offFile)
    {
        string page;
        page.resize(elem.second);
        ifs.seekg(elem.first, std::ios::beg);
        ifs.read(&page[0], elem.second);
        filePage.push_back(page);
    }

    for (auto &xml : filePage)
    {
        XMLDocument doc;
        doc.Parse(xml.c_str());

        // 获取 <url> 和 <title> 节点
        XMLElement *urlElement = doc.FirstChildElement("doc")->FirstChildElement("url");
        XMLElement *titleElement = doc.FirstChildElement("doc")->FirstChildElement("title");

        if (urlElement && titleElement)
        {
            const char *url = urlElement->GetText();
            const char *title = titleElement->GetText();

            res.emplace_back(url, title);
        }
    }

    ifs.close();
    return res;
}

//更新引擎缓存
void Engine::updatCache()
{
    _cacheManager.periodicUpdateCaches();
}

size_t Engine::getByteNum_UTF8(const char byte)
{
    int byteNum = 0;
    for (size_t i = 0; i < 6; ++i)
    {
        if (byte & (1 << (7 - i)))
            ++byteNum;
        else
            break;
    }
    return byteNum == 0 ? 1 : byteNum;
}
