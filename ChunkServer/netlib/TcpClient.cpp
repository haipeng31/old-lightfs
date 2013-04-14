#include "TcpClient.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Connector.h"
#include "base/Logging.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace mynet;

TcpClient::TcpClient(EventLoop *loop, const char *peerAddr, int peerPort, bool retry)
{
	loop_ = loop;
	connector_ = new Connector(loop, peerAddr, peerPort, retry);
	connection_ = NULL;
	fd_ = connector_->fd();
	retry_ = retry;
	connected_ = false;
	memset(peerAddr_, 0, sizeof(peerAddr_));
	strncpy(peerAddr_, peerAddr, strlen(peerAddr));
	peerPort_ = peerPort;
	
	LOG_INFO << "Client initialized";
}

TcpClient::~TcpClient()
{
	delete connector_;
}

void TcpClient::setConnectCallback(Callback connectCallback, void *connectArgs)
{
	connectCallback_ = connectCallback;
	connectArgs_ = connectArgs;
}

void TcpClient::setMessageCallback(Callback messageCallback, void *messageArgs)
{
	messageCallback_ = messageCallback;
	messageArgs_ = messageArgs;
}

void TcpClient::setCloseCallback(Callback closeCallback, void *closeArgs)
{
	closeCallback_ = closeCallback;
	closeArgs_ = closeArgs;
}

void TcpClient::setErrorCallback(Callback errorCallback, void *errorArgs)
{
	errorCallback_ = errorCallback;
	errorArgs_ = errorArgs;
}

void TcpClient::setWriteCompleteCallback(Callback writeCompleteCallback, void *writeCompleteArgs)
{
	writeCompleteCallback_ = writeCompleteCallback;
	writeCompleteArgs_ = writeCompleteArgs;
}

void TcpClient::start()
{
	assert(connected_ == false);
	connector_->setConnectionCallback(handleWrite, (void *)this);
	connector_->start();

	LOG_INFO << "Client Try to connect Server " << peerAddr_ << ":" << peerPort_;
}


void TcpClient::handleWrite(void *args, TcpConnection *connection)
{
	TcpClient *tcpClient = (TcpClient *)args;
	tcpClient->handleWrite(connection);
}

void TcpClient::handleWrite(TcpConnection *connection)
{
	connection_ = connection;
	connectCallback_(connectArgs_, connection_);
	LOG_INFO << "Successfully connect to server";

	connection_->setMessageCallback(messageCallback_);
	connection_->setMessageArgs(messageArgs_);

	connection_->setErrorCallback(errorCallback_);
	connection_->setErrorArgs(errorArgs_);

	connection_->setCloseCallback(handleClose);
	connection_->setCloseArgs((void *)this);

	connection_->setWriteCompleteCallback(writeCompleteCallback_);
	connection_->setWriteCompleteArgs(writeCompleteArgs_);
}

void TcpClient::handleClose(void *args, TcpConnection *connection)
{
	TcpClient *tcpClient = (TcpClient *)args;
	tcpClient->handleClose(connection);
}

void TcpClient::handleClose(TcpConnection *connection)
{
	assert(connected_ == true);
	connected_ = false;

	closeCallback_(closeArgs_, connection);
	delete connection;
	connection_ = NULL;
}


// to do list
// TcpConnection: add writeComplete callback, done
// TcpClient: add writeComplete callback
// testclient: finish the test code
