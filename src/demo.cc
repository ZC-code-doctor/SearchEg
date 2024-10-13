
#include "PageLibPreprocessor.h"
#include "Configuration.h"
#include <iostream>

int main()
{
    Configuration* pConf = Configuration::getInstance("../conf/Myconf.conf");
    PageLibPreprocessor PageLib(pConf);
    return 0;
}