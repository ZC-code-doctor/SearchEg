all:test

test:../src/SplitEn.o ../src/SplitTool.o ../src/DictProducer.o ../src/main.o
	g++ ../src/main.o ../src/DictProducer.o ../src/SplitEn.o ../src/SplitTool.o -o test
../src/main.o:../src/main.cc
	g++ -c ../src/main.cc -o ../src/main.o -g -I ../include
../src/DictProducer.o:../src/DictProducer.cc
	g++ -c ../src/DictProducer.cc -o ../src/DictProducer.o -g -I ../include
../src/SplitEn.o:../src/SplitEn.cc
	g++ -c ../src/SplitEn.cc -o ../src/SplitEn.o -g -I ../include
../src/SplitTool.o:../src/SplitTool.cc
	g++ -c ../src/SplitTool.cc -o ../src/SplitTool.o -g -I ../include
