#include "EventLoop.h"
#include "Channel.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//#include "Buffer.h"
#include "TcpConnection.h"
#define SERV_PORT 5555
using mynet::EventLoop;
using mynet::Channel;
using mynet::Buffer;
using mynet::TcpConnection;

void setnonblocking(int sock)
{
	int opts;
	opts = fcntl(sock, F_GETFL);

	if (opts < 0) {
		perror("Error in fcntl");
		exit(1);
	}

	opts = opts | O_NONBLOCK;

	if (fcntl(sock, F_SETFL, opts) < 0) {
		perror("Erro in fcntl");
		exit(1);
	}
}

struct Arg {
	Channel *channel;
	EventLoop *loop;
};

void onMessage(void *args)
{
	TcpConnection *con = (TcpConnection *)args;
	char temp[1024];
	memset(temp, 0, 1024);
	int n = con->getInputBuf()->readBuf(temp, con->getInputBuf()->avail());
	printf("%s\n", temp);
	con->sendMsg(temp, n); 
}
void onClose(void *args)
{
	printf("close socket\n");
}
void onConnection(void *args)
{
	Arg *arg = (Arg *)args;
	Channel *channel = arg->channel;
	EventLoop *loop = arg->loop;
struct sockaddr_in clientaddr;
socklen_t clilen;
	int connfd = accept(channel->fd(), (struct sockaddr *)&clientaddr, &clilen);
	printf("new connection\n");
	TcpConnection *con = new TcpConnection(connfd, loop);
	con->setMessageCallback(onMessage);
	con->setCloseCallback(onClose);
}

int main()
{
        int i, listenfd, connfd;
	int rt;
	ssize_t n;
	
	socklen_t clilen;

	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
	setnonblocking(listenfd);
	/*
	 * set and bind ip and port
	 */
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERV_PORT);
	bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	
	/*
	 * start to waiting for connect request
	 */
	listen(listenfd, 10);
	
	EventLoop loop;
	Channel channel(listenfd);
	channel.setEvent(Channel::kReadEvent);
	//Test test;
	Arg args;
	args.channel = &channel;
	args.loop = &loop;
	channel.setArgs((void *)&args);
	channel.setReadCallback(onConnection);
	loop.addChannel(&channel);

	loop.loop();
	return 0;
}
