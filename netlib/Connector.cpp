#include "Connector.h"
#include "base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "SockOps.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

using namespace mynet;

Connector::Connector(EventLoop *loop, const char *peerAddr, int peerPort, bool retry)
{
	loop_ = loop;
	fd_ = socket(); 
	channel_ = new Channel(fd_);

	memset(peerAddr_, 0, sizeof(peerAddr_));
	strncpy(peerAddr_, peerAddr, strlen(peerAddr));
	peerPort_ = peerPort;

	connected_ = false;
	retry_ = retry;
}

Connector::~Connector()
{
	delete channel_;
}

void Connector::setConnectionCallback(Callback connectionCallback, void *args)
{
	connectionCallback_ = connectionCallback;
	connectionArgs_ = args;
}

void Connector::start()
{
	assert(connected_ == false);
	int n = connect(fd_, peerAddr_, peerPort_);
	
	if (n == 0) {
		TcpConnection *connection = new TcpConnection(fd_, loop_, peerAddr_, peerPort_);
		connected_ = true;
		connectionCallback_(connectionArgs_, connection);
	} else {
		int saveErrNo = errno;
		if (saveErrNo == EINPROGRESS || saveErrNo == EINTR) {
			connecting();
		}
	}
}

void Connector::connecting()
{
	assert(connected_ == false);
	channel_->setEvent(Channel::kWriteEvent);
	channel_->setWriteCallback(handleWrite);
	channel_->setArgs((void *)this);
	loop_->addChannel(channel_);
}

void Connector::handleWrite(void *args)
{
	Connector *connector = (Connector *)args;
	connector->handleWrite();
}

int Connector::fd()
{
	return fd_;
}
/* fix me
 * need to deal with the situation that connect fail
 */
void Connector::handleWrite()
{
	loop_->removeChannel(channel_);
	TcpConnection *connection = new TcpConnection(fd_, loop_, peerAddr_, peerPort_);
	connected_ = true;
	connectionCallback_(connectionArgs_, connection);
}
