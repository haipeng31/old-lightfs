#include "Acceptor.h"
#include "base/Logging.h"
#include "EventLoop.h"
#include <string.h>
#include "SockOps.h"
#include <stdlib.h>
#include "TcpConnection.h"
#include <assert.h>

using namespace mynet;

Acceptor::Acceptor(EventLoop *loop, const char *ipAddr, int port)
 :  loop_(loop),
	listenfd_(socket()),
	channel_(listenfd_)
{
	loop_ = loop;
	connectionCallback_ = NULL;
	connectionArgs_ = NULL;
	memset(ipAddr_, 0, sizeof(ipAddr_));
	strncpy(ipAddr_, ipAddr, strlen(ipAddr));
	port_ = port;
	started_ = false;

}

Acceptor::~Acceptor()
{
	close(listenfd_);
}

void Acceptor::start()
{
	assert(started_ == false);
	started_ = false;
	
	setNonBlocking(listenfd_);
	bind(listenfd_, port_);
	listen(listenfd_, 10);

	channel_.setEvent(Channel::kReadEvent);
	channel_.setReadCallback(readCallback);
	channel_.setArgs((void *)this);
	loop_->addChannel(&channel_);
	LOG_INFO << "Acceptor started";
}
void Acceptor::setConnectionCallback(Callback connectionCallback, void *args)
{
	connectionCallback_ = connectionCallback;
	connectionArgs_ = args;
}

void Acceptor::readCallback(void *args)
{
	Acceptor *acceptor = (Acceptor *)args;
	acceptor->handleRead();
}

void Acceptor::handleRead()
{
	char *peerAddr = (char *)malloc(20 * sizeof(char));
	memset(peerAddr, 0, 20 * sizeof(char));
	int peerPort = 0;
	int sockfd = accept(listenfd_, peerAddr, &peerPort);
	TcpConnection *con = new TcpConnection(sockfd, loop_, peerAddr, peerPort);	
	if (sockfd > 0) {
		LOG_INFO << "new connection";
		connectionCallback_(connectionArgs_, con);
	}
}
