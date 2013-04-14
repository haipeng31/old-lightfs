test:test.o Logging.o ThreadPool.o
	g++ test.o Logging.o ThreadPool.o -lpthread -o test
test.o:test.cpp
	g++ -c test.cpp
ThreadPool.o:ThreadPool.cpp
	g++ -c ThreadPool.cpp
Logging.o:Logging.cpp
	g++ -c Logging.cpp
