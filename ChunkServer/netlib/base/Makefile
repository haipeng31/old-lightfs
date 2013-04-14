test:test.o Logging.o ThreadPool.o FileSystem.pb.o
	g++ test.o Logging.o ThreadPool.o FileSystem.pb.o -lpthread -lprotoc -o test
test.o:test.cpp
	g++ -c test.cpp

FileSystem.pb.o:../../FileSystem.pb.cc
	g++ -c ../../FileSystem.pb.cc
ThreadPool.o:ThreadPool.cpp
	g++ -c ThreadPool.cpp
Logging.o:Logging.cpp
	g++ -c Logging.cpp
