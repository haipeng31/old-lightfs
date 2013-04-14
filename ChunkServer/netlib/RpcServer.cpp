#include "RpcServer.h"
#include "base/Logging.h"

using namespace mynet;

RpcServer::RpcServer(EventLoop *loop, const char *name, const char *ipAddr, int port, int nthread, int nwriteThread)
 :  threadPool_(nthread, nwriteThread),
	server_(loop, name, ipAddr, port),
	codec_(4, &threadPool_),
	dispatcher_()
{
	LOG_INFO << "RPC SERVER " << name << " initialized";
}

RpcServer::~RpcServer()
{
}

void RpcServer::start()
{
	threadPool_.start();
	codec_.setMessageCallback(Dispatcher::dispatchMessage, &dispatcher_);
	server_.setMessageCallback(Codec::dealMsg, &codec_);
	server_.setConnectionCallback(onConnection, NULL);
	server_.setCloseCallback(onClose, NULL);
	//server_.setErrorCallback(onError, NULL);

	server_.start();
}

void RpcServer::registerRpc(int messageType, MessageCallback messageCallback, void *objArg)
{
	dispatcher_.registerMsgCallback(messageType, messageCallback, objArg);
}

void RpcServer::onConnection(void *objArg, TcpConnection *con)
{
	LOG_INFO << "New connection from " << con->peerAddr() << ":" << con->peerPort();
}

void RpcServer::onClose(void *objArg, TcpConnection *con)
{
	LOG_INFO << "Connection " << con->peerAddr() << ":" << con->peerPort() << " down";
}

void RpcServer::onError(void *objArg, TcpConnection *con)
{
	LOG_INFO << "Connection " << con->peerAddr() << ":" << con->peerPort() << " error";
}
