#include "SplitTool.h"
#include "SplitEn.h"
#include "DictProducer.h"

#include <iostream>


int main()
{
    SplitEn dict;
    DictProducer temp("../yuliao/en_yuliao/",&dict);
    temp.showFile();


    return 0;
}

