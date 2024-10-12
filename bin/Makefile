all:test

test:../src/SplitEn.o ../src/SplitTool.o ../src/DictProducer.o ../src/main.o
	g++ ../src/main.o ../src/DictProducer.o ../src/SplitEn.o ../src/SplitTool.o -o test
../src/main.o:../src/main.cc
	g++ -c -g ../src/main.cc -o ../src/main.o -I ../include
../src/DictProducer.o:../src/DictProducer.cc
	g++ -c -g ../src/DictProducer.cc -o ../src/DictProducer.o -I ../include
../src/SplitEn.o:../src/SplitEn.cc
	g++ -c -g ../src/SplitEn.cc -o ../src/SplitEn.o -I ../include
../src/SplitTool.o:../src/SplitTool.cc
	g++ -c -g ../src/SplitTool.cc -o ../src/SplitTool.o -I ../include
