#include "SplitTool.h"
#include "SplitEn.h"
#include "DictProducer.h"
#include "Configuration.h"
#include "SplitZh.h"

#include <iostream>


int main()
{
    Configuration* pConf = Configuration::getInstance("../conf/Myconf.conf");
    //预热阶段,加载关键字、网页资源
    {
        SplitEn en;
        SplitZh zh;
        DictProducer temp(pConf,&en,&zh);
    }

    std::cout<<"whosyourdaddy"<<"\n";
    
    return 0;
}

