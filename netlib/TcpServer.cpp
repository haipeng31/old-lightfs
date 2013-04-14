#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "base/Logging.h"
#include <string.h>
#include <assert.h>

using namespace mynet;

TcpServer::TcpServer(EventLoop *loop, const char *name, const char *ipAddr, int port)
{
	loop_ = loop;
	acceptor_ = new Acceptor(loop, ipAddr, port);
	memset(name_, 0, 50);
	strncpy(name_, name, strlen(name));
	port_ = port;
	messageCallback_ = NULL;
	connectionCallback_ = NULL;
	messageArgs_ = NULL;
	connectionArgs_ = NULL;
	started_ = false;
	LOG_INFO << "server " << name_ << " initialized on port " << port_;
}

TcpServer::~TcpServer()
{
	connections_.clear();
	delete acceptor_;
}

void TcpServer::start()
{
	assert(started_ == false);
	started_ = true;

	acceptor_->setConnectionCallback(handleNewConnection, this);
	acceptor_->start();
	LOG_INFO << "server " << name_ << " started";
}

void TcpServer::setMessageCallback(MessageCallback messageCallback, void *messageArgs)
{
	messageCallback_ = messageCallback;
	messageArgs_ = messageArgs;
}

void TcpServer::setCloseCallback(CloseCallback closeCallback, void *closeArgs)
{
	closeCallback_ = closeCallback;
}

void TcpServer::handleClose(void *args, TcpConnection *con)
{
	TcpServer *server = (TcpServer *)args;
	server->handleClose(con);
}

void TcpServer::handleClose(TcpConnection *con)
{
	assert(connections_[con->fd()] == con);
	connections_.erase(con->fd());

	if (closeCallback_) {
		closeCallback_(closeArgs_, con);
	}
	//delete con;
}
void TcpServer::setConnectionCallback(ConnectionCallback connectionCallback, void *connectionArgs)
{
	connectionCallback_ = connectionCallback;
	connectionArgs_ = connectionArgs;
}

void TcpServer::handleNewConnection(void *args, TcpConnection *con)
{
	TcpServer *server = (TcpServer *)args;
	server->handleNewConnection(con);
}

void TcpServer::handleNewConnection(TcpConnection *connection)
{
	assert(connection != NULL);
	connection->setMessageCallback(messageCallback_);
	connection->setMessageArgs(messageArgs_);
	connection->setCloseCallback(handleClose);
	connection->setCloseArgs((void *)this);
	assert(connections_.find(connection->fd()) == connections_.end());
	connections_[connection->fd()] = connection;
	
	connectionCallback_(connectionArgs_, connection);
	//LOG_INFO << "New Connection from " << peerAddr << ":" << peerPort;
}

// to do list
// implement the func to deal with new connection establishing,
// using Acceptor to bind
// bind messageCallback in the implemented fucntion,
// maybe we should change the definiton of TcpConnection's messageCallback
