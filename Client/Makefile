OBJ = CSConManager.o FileSystem.pb.o FsClient.o FsRpcClient.o
NETOBJ = ../netlib/TcpBlockingClient.o ../netlib/Buffer.o ../netlib/base/Logging.o ../netlib/SockOps.o

fsclient:$(OBJ) $(NETOBJ) fuse.o
	g++ $(OBJ) $(NETOBJ) fuse.o -lfuse -lprotobuf -o fsclient
testserver:$(OBJ) $(NETOBJ) testserver.o
	g++ $(OBJ) $(NETOBJ) testserver.o -lprotobuf -o testserver
testchunk:$(OBJ) $(NETOBJ) testchunk.o
	g++ $(OBJ) $(NETOBJ) testchunk.o -lprotobuf -o testchunk

testchunk.o:testchunk.cpp
	g++ -c testchunk.cpp
testserver.o:testserver.cpp
	g++ -c testserver.cpp
fuse.o:fuse.cpp
	g++ -c -D_FILE_OFFSET_BITS=64 fuse.cpp

CSConManager.o:CSConManager.cpp
	g++ -c CSConManager.cpp

FileSystem.pb.o:FileSystem.pb.cc
	g++ -c FileSystem.pb.cc

FsClient.o:FsClient.cpp
	g++ -c FsClient.cpp

FsRpcClient.o:FsRpcClient.cpp
	g++ -c FsRpcClient.cpp
