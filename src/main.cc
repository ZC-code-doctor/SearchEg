#include "SplitTool.h"
#include "SplitEn.h"
#include "DictProducer.h"

#include <iostream>


int main()
{
    SplitEn dict;
    DictProducer temp("../conf/Myconf.conf",&dict);
    return 0;
}

