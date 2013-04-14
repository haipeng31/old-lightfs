OBJ = CSConManager.o ChunkServer.o ChunkManager.o LeaseManager.o CacheDataManager.o FileSystem.pb.o
CLIENTOBJ = FsClient.o FsRpcClient.o
NETOBJ = netlib/TimerQueue.o netlib/Timer.o netlib/Acceptor.o netlib/Buffer.o netlib/Channel.o netlib/Codec.o netlib/Connector.o netlib/Dispatcher.o netlib/EPoller.o netlib/EventLoop.o netlib/Poller.o netlib/RpcClient.o netlib/RpcServer.o netlib/SockOps.o netlib/TcpBlockingClient.o netlib/TcpClient.o netlib/TcpConnection.o netlib/TcpServer.o
BASEOBJ = netlib/base/Logging.o netlib/base/ThreadPool.o

chunkserver:$(OBJ) $(NETOBJ) $(BASEOBJ) $(CLIENTOBJ) main.o
	g++ $(OBJ) $(NETOBJ) $(BASEOBJ) $(CLIENTOBJ) main.o -lrt -lprotobuf -o chunkserver
testcache:$(OBJ) $(BASEOBJ) testcache.o
	g++ $(OBJ) $(BASEOBJ) testcache.o -lrt -o testcache

testlease:$(OBJ) $(BASEOBJ) testlease.o
	g++ $(OBJ) $(BASEOBJ) testlease.o -lrt -o testlease
testchunk:$(OBJ) $(BASEOBJ) testchunk.o
	g++ $(OBJ) $(BASEOBJ) testchunk.o -o testchunk

testcache.o:testcache.cpp
	g++ -c -g testcache.cpp
testlease.o:testlease.cpp
	g++ -c -g testlease.cpp

testchunk.o:testchunk.cpp
	g++ -c -g testchunk.cpp

FsClient.o:FsClient.cpp
	g++ -c -g FsClient.cpp

FsRpcClient.o:FsRpcClient.cpp
	g++ -c -g FsRpcClient.cpp

CSConManager.o:CSConManager.cpp
	g++ -c -g CSConManager.cpp

FileSystem.pb.o:FileSystem.pb.cc
	g++ -c -g FileSystem.pb.cc

ChunkServer.o:ChunkServer.cpp
	g++ -c -g ChunkServer.cpp

CacheDataManager.o:CacheDataManager.cpp
	g++ -c -g CacheDataManager.cpp
LeaseManager.o:LeaseManager.cpp
	g++ -c -g LeaseManager.cpp
ChunkManager.o:ChunkManager.cpp
	g++ -c -g ChunkManager.cpp

clean:
	rm *.o
