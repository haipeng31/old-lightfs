OBJ = MetaServer.o MetaDataManager.o ChunkServerState.o DirTree.o HashTable.o INode.o ChunkInfo.o

NETOBJ = netlib/Acceptor.o netlib/Buffer.o netlib/Channel.o netlib/Codec.o netlib/Connector.o netlib/Dispatcher.o netlib/EPoller.o netlib/EventLoop.o netlib/Poller.o netlib/RpcClient.o netlib/RpcServer.o netlib/SockOps.o netlib/TcpBlockingClient.o netlib/TcpClient.o netlib/TcpConnection.o netlib/TcpServer.o
BASEOBJ = netlib/base/Logging.o netlib/base/ThreadPool.o
CLIENTOBJ = Client/FileSystem.pb.o

main:$(OBJ) $(BASEOBJ) $(NETOBJ) $(CLIENTOBJ) main.o
	g++ $(OBJ) $(BASEOBJ) $(NETOBJ) $(CLIENTOBJ) main.o -lprotobuf -o main

testdir.o:testdir.cpp
	g++ -c -g -Wall testdir.cpp

main.o:main.cpp
	g++ -c -g -Wall main.cpp

MetaServer.o:MetaServer.cpp
	g++ -c -g -Wall MetaServer.cpp

MetaDataManager.o:MetaDataManager.cpp
	g++ -c -g -Wall MetaDataManager.cpp

ChunkServerState.o:ChunkServerState.cpp
	g++ -c -g -Wall ChunkServerState.cpp

DirTree.o:DirTree.cpp
	g++ -c -g -Wall DirTree.cpp

HashTable.o:HashTable.cpp
	g++ -c -g -Wall HashTable.cpp

INode.o:INode.cpp
	g++ -c -g -Wall INode.cpp

ChunkInfo.o:ChunkInfo.cpp
	g++ -c -g -Wall ChunkInfo.cpp
