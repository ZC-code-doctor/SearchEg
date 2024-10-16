INCLUDES = -I include
CXXFLAGS = -c -g $(INCLUDES)
LDFLAGS = -lm -pthread -lredis++ -lhiredis

all: server

# 通用规则: 将生成的 .o 文件放到 bin/ 目录
bin/%.o: src/%.cc
	g++ $(CXXFLAGS) $< -o $@
# 为 tinyxml2.cpp 添加编译规则
bin/tinyxml2.o: src/tinyxml2.cpp
	g++ $(CXXFLAGS) $< -o $@
# 具体目标

#离线资源处理程序
LoadResource: bin/LoadResource.o bin/PageLibPreprocessor.o bin/WebPage.o \
              bin/Configuration.o bin/tinyxml2.o bin/Rss.o bin/Tools.o \
              bin/SplitEn.o bin/SplitTool.o bin/SplitZh.o bin/DictProducer.o
	g++ $^ -o LoadResource $(LDFLAGS)

#服务器
server:bin/Acceptor.o bin/AcceptServer.o bin/Factory.o bin/InetAdderess.o \
	   bin/IOserver.o bin/Socket.o bin/SocketIO.o bin/Task.o bin/TaskQueue.o \
	   bin/Tcpconnection.o bin/Tcp_server.o bin/ThreadPool.o \
	   bin/Engine.o bin/Configuration.o bin/WebPage.o bin/Tools.o bin/tinyxml2.o \
	   bin/LRUcache.o \
	   bin/server.o
	g++ $^ -o server $(LDFLAGS)

#单独引擎测试
Engine:bin/Engine.o bin/Configuration.o bin/WebPage.o bin/Tools.o bin/tinyxml2.o bin/LRUcache.o bin/test.o
	g++ $^ -o Engine $(LDFLAGS)