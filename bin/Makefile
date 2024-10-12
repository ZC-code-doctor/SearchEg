all:test

test:../src/SplitEn.o ../src/SplitTool.o ../src/SplitZh.o ../src/DictProducer.o ../src/Configuration.o ../src/main.o
	g++ ../src/main.o ../src/DictProducer.o ../src/SplitEn.o ../src/SplitZh.o ../src/SplitTool.o ../src/Configuration.o -o test
../src/main.o:../src/main.cc
	g++ -c -g ../src/main.cc -o ../src/main.o -I ../include
../src/Configuration.o:../src/Configuration.cc ../include/Configuration.h
	g++ -c -g ../src/Configuration.cc -o ../src/Configuration.o -I ../include
../src/DictProducer.o:../src/DictProducer.cc ../include/DictProducer.h
	g++ -c -g ../src/DictProducer.cc -o ../src/DictProducer.o -I ../include
../src/SplitZh.o:../src/SplitZh.cc ../include/SplitZh.h
	g++ -c -g ../src/SplitZh.cc -o ../src/SplitZh.o -I ../include
../src/SplitEn.o:../src/SplitEn.cc ../include/SplitEn.h
	g++ -c -g ../src/SplitEn.cc -o ../src/SplitEn.o -I ../include
../src/SplitTool.o:../src/SplitTool.cc ../include/SplitTool.h
	g++ -c -g ../src/SplitTool.cc -o ../src/SplitTool.o -I ../include
