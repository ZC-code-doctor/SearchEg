#include "SplitTool.h"
#include "SplitEn.h"
#include "DictProducer.h"
#include "Configuration.h"

#include <iostream>


int main()
{
    Configuration* pConf = Configuration::getInstance("../conf/Myconf.conf");
    SplitEn dict;
    DictProducer temp(pConf,&dict);
    
    return 0;
}

