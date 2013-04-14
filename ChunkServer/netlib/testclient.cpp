#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpClient.h"
#include "base/Logging.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace mynet;
void onConnection(void *args, TcpConnection *con)
{
	// do nothing
	char msg[50];
	memset(msg, 0, sizeof(msg));
	scanf("%s", msg);
	con->sendMsg(msg, strlen(msg));
}

void onMessage(void *args, TcpConnection *con)
{
	char msg[50];
	memset(msg, 0, sizeof(msg));
	int n = con->getInputBuf()->readBuf(msg, 50);
	LOG_INFO << msg;
}

void onClose(void *args, TcpConnection *con)
{
	LOG_INFO << "client down";
	// do nothing
}

void onError(void *args, TcpConnection *con)
{
	// do nothing
}

void onWriteComplete(void *args, TcpConnection *con)
{
	char msg[50];
	memset(msg, 0, sizeof(msg));
	scanf("%s", msg);
	con->sendMsg(msg, strlen(msg));
}

int main()
{
	EventLoop loop;
	TcpClient client(&loop, "127.0.0.1", 5555);

	client.setConnectCallback(onConnection, NULL);
	client.setMessageCallback(onMessage, NULL);
	client.setCloseCallback(onClose, NULL);
	client.setErrorCallback(onError, NULL);
	client.setWriteCompleteCallback(onWriteComplete, NULL);
	
	client.start();
	loop.loop();
}


