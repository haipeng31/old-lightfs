OBJ = Timer.o TimerQueue.o TcpBlockingClient.o RpcClient.o EchoClient.o Codec.o Dispatcher.o RpcServer.o TcpClient.o Connector.o TcpServer.o Acceptor.o TcpConnection.o SockOps.o EventLoop.o Channel.o Poller.o EPoller.o Buffer.o ../FileSystem.pb.o
BASEOBJ = base/Logging.o base/ThreadPool.o

testtimer:$(OBJ) $(BASEOBJ) testtimer.o
	g++ $(OBJ) $(BASEOBJ) testtimer.o -DTRACE -lrt -lpthread -lprotoc -o testtimer
testrpc:$(OBJ) $(BASEOBJ) testrpc.o
	g++ $(OBJ) $(BASEOBJ) testrpc.o -lpthread -o testrpc
testrpcclient:$(OBJ) $(BASEOBJ) testrpcclient.o
	g++ $(OBJ) $(BASEOBJ) testrpcclient.o -o testrpcclient
client:$(OBJ) $(BASEOBJ) testclient.o
	g++ $(OBJ) $(BASEOBJ) testclient.o -o client
server: $(OBJ) $(BASEOBJ) testserver.o
	g++ $(OBJ) $(BASEOBJ) testserver.o -o server
testsockops.o:testsockops.cpp
	g++ -c -g testsockops.cpp

testsockopsclient.o:testsockopsclient.cpp
	g++ -c -g testsockopsclient.cpp

testevent.o:testevent.cpp
	g++ -c -g testevent.cpp

testrpcclient.o:testrpcclient.cpp
	g++ -c -g testrpcclient.cpp

testtimer.o:testtimer.cpp
	g++ -c -g testtimer.cpp

Timer.o:Timer.cpp
	g++ -c -g Timer.cpp

TimerQueue.o:TimerQueue.cpp
	g++ -c -g TimerQueue.cpp

TcpBlockingClient.o:TcpBlockingClient.cpp
	g++ -c -g TcpBlockingClient.cpp

RpcClient.o:RpcClient.cpp
	g++ -c -g RpcClient.cpp

EchoClient.o:EchoClient.cpp
	g++ -c -g EchoClient.cpp
testrpc.o:testrpc.cpp
	g++ -c -g testrpc.cpp

Codec.o:Codec.cpp
	g++ -c -g Codec.cpp

Dispatcher.o:Dispatcher.cpp
	g++ -c -g Dispatcher.cpp

RpcServer.o:RpcServer.cpp
	g++ -c -g RpcServer.cpp

testclient.o:testclient.cpp
	g++ -c -g testclient.cpp

testserver.o:testserver.cpp
	g++ -c -g testserver.cpp

TcpClient.o:TcpClient.cpp
	g++ -c -g TcpClient.cpp

Connector.o:Connector.cpp
	g++ -c -g Connector.cpp

TcpServer.o:TcpServer.cpp
	g++ -c -g TcpServer.cpp
Acceptor.o:Acceptor.cpp
	g++ -c -g Acceptor.cpp
TcpConnection.o:TcpConnection.cpp
	g++ -c -g TcpConnection.cpp
SockOps.o:SockOps.cpp
	g++ -c -g SockOps.cpp
testbuffer.o:testbuffer.cpp
	g++ -c -g testbuffer.cpp

EventLoop.o:EventLoop.cpp
	g++ -c -g EventLoop.cpp

Channel.o:Channel.cpp
	g++ -c -g Channel.cpp

Poller.o:Poller.cpp
	g++ -c -g Poller.cpp

EPoller.o:EPoller.cpp
	g++ -c -g EPoller.cpp

Buffer.o:Buffer.cpp
	g++ -c -g Buffer.cpp
clean:
	rm *.o
