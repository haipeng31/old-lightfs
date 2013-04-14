#include "TcpConnection.h"
#include "TcpServer.h"
#include "base/Logging.h"
//#include "Buffer.h"
#include <stdlib.h>
#include <string.h>
#include "EventLoop.h"

using namespace mynet;

void onMessage(void *arg, TcpConnection *con)
{
	char msg[100];
	memset(msg, 0, sizeof(msg));
	int n = con->getInputBuf()->readBuf(msg, sizeof(msg));
	con->sendMsg(msg, n);
}

void onConnection(void *arg, TcpConnection *con)
{
	LOG_INFO << "New connection from " << con->peerAddr() << ":" << con->peerPort();
}

void onClose(void *arg, TcpConnection *con)
{
	LOG_INFO << "Connection " << con->peerAddr() << ":" << con->peerPort() << " down";
}

int main()
{
	EventLoop loop;
	TcpServer server(&loop, "ECHO-Server", "127.0.0.1", 5555);
	server.setMessageCallback(onMessage, NULL);
	server.setConnectionCallback(onConnection, NULL);
	server.setCloseCallback(onClose, NULL);
	server.start();
	loop.loop();
	return 0;
}
