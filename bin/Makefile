INCLUDES = -I ../include
CXXFLAGS = -c -g $(INCLUDES)

all: Engine

# 通用规则
%.o: %.cc
	g++ $(CXXFLAGS) $< -o $@

# 具体目标
loadResource: ../src/LoadResource.o ../src/PageLibPreprocessor.o ../src/WebPage.o \
              ../src/Configuration.o ../src/tinyxml2.o ../src/Rss.o \
              ../src/SplitEn.o ../src/SplitTool.o ../src/SplitZh.o ../src/DictProducer.o
	g++ $^ -o loadResource


Engine:../src/Engine.o ../src/Configuration.o ../src/WebPage.o ../src/test.o
	g++ $^ -o Engine