#include "Factory.h"

Task* Factory::productTask(string val)
{
    Task* pA= new TaskA(val);
    return pA;
}

Task* Factory::productTask(int val)
{
    Task* pB= new TaskB(val);
    return pB;
}

Task* Factory::productTask(Packge pack)
{
    Task* pC= new TaskC(pack);
    return pC;
}


//本项目任务类型
Task* Factory::productTask(factor fc)
{
    Task* pD= new TaskD(std::move(fc));
    return pD;
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







