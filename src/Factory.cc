#include "Factory.h"


//本项目任务类型
Task* Factory::productTask(factor fc)
{
    Task* pSend= new SendTask(std::move(fc));
    return pSend;
}

Task* Factory::productTask(HttpPackge pack)
{
    Task* pHTTP= new HttpTask(pack);
    return pHTTP;
}

Task* Factory::productTask(SearchTask pack)
{
    Task* pSearch= new SearchTask(pack);
    return pSearch;
}

Task* Factory::productTask(RecommendTask pack)
{
    Task* pRecommend= new RecommendTask(pack);
    return pRecommend;
}







