#include "Search.h"
#include<iostream>

using std::cout;

int main()
{
    Configuration *pConf =  Configuration::getInstance("../conf/Myconf.conf");
    Search sec(pConf);
    sec.SearchPage();
    return 0;
}