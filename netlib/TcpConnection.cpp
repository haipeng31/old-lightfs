#include "TcpConnection.h"
#include "EventLoop.h"
#include "base/Logging.h"
#include "SockOps.h"
#include <stdlib.h>
#include <string.h>
using namespace mynet;

TcpConnection::TcpConnection(int sockfd, EventLoop *loop, const char *peerAddr, int peerPort)
 :  fd_(sockfd),
	channel_(sockfd),
	loop_(loop),
	inputBuffer_(),
	outputBuffer_(),
	messageCallback_(NULL),
	closeCallback_(NULL),
	errorCallback_(NULL),
	writeCompleteCallback_(NULL)
{
	memset(peerAddr_, 0, sizeof(peerAddr_));
	strncpy(peerAddr_, peerAddr, strlen(peerAddr));
	peerPort_ = peerPort; 
	channel_.setEvent(Channel::kReadEvent);
	//channel_.setEvent(Channel::kWriteEvent);
	channel_.setArgs((void *)this);
	channel_.setReadCallback(readCallback);
	//channel_.setWriteCallback(writeCallback);
	channel_.setErrorCallback(errorCallback);
	loop_->addChannel(&channel_);
	LOG_INFO << "Add Channel with fd " << fd_;
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::setMessageCallback(Callback messageCallback)
{
	messageCallback_ = messageCallback;
}

void TcpConnection::setCloseCallback(Callback closeCallback)
{
	closeCallback_ = closeCallback;
}

void TcpConnection::setErrorCallback(Callback errorCallback)
{
	errorCallback_ = errorCallback;
}

void TcpConnection::setWriteCompleteCallback(Callback writeCompleteCallback)
{
	writeCompleteCallback_ = writeCompleteCallback;
}

/* many thread update the eventloop, need lock, fix me */
void TcpConnection::sendMsg(void *data, int len)
{
	LOG_TRACE << "outputBuffer avail = " << outputBuffer_.avail();
	outputBuffer_.writeBuf((char *)data, len);
	LOG_TRACE << "outputBuffer avail = " << outputBuffer_.avail();
	channel_.setEvent(Channel::kWriteEvent);
	channel_.setWriteCallback(writeCallback);
	loop_->updateChannel(&channel_);
}

Buffer *TcpConnection::getInputBuf()
{
	return &inputBuffer_;
}

Buffer *TcpConnection::getOutputBuf()
{
	return &outputBuffer_;
}
/* if the callback is a function of a class, we 
 * need args, if it is a normal function, args = NULL
 */
void TcpConnection::setMessageArgs(void *args)
{
	messageArgs_ = args;
}

void TcpConnection::setCloseArgs(void *args)
{
	closeArgs_ = args;
}

void TcpConnection::setErrorArgs(void *args)
{
	closeArgs_ = args;
}

void TcpConnection::setWriteCompleteArgs(void *args)
{
	writeCompleteArgs_ = args;	
}

int TcpConnection::fd()
{
	return fd_;
}
const char *TcpConnection::peerAddr()
{
	return peerAddr_;
}

int TcpConnection::peerPort()
{
	return peerPort_;
}

void TcpConnection::readCallback(void *args)
{
	LOG_TRACE << "readcallback";
	TcpConnection *con = (TcpConnection *)args;
	int n = con->handleRead();
	if (n == 0) {
		//delete con;
	}
}

void TcpConnection::writeCallback(void *args)
{
	LOG_TRACE << "writecallback";
	TcpConnection *con = (TcpConnection *)args;
	con->handleWrite();
}

void TcpConnection::errorCallback(void *args)
{
	TcpConnection *con = (TcpConnection *)args;
	con->handleError();
}

int TcpConnection::handleRead()
{
	int n = inputBuffer_.bufReadFd(fd_);
	if (n == 0) {
		handleClose();
	} else if (n > 0) {
		messageCallback_(messageArgs_, this);
	} else {
		// do nothing
	}

	return n;

}

void TcpConnection::handleWrite()
{
	outputBuffer_.showContent();
	LOG_TRACE << "-----------------------------";
	LOG_TRACE << "-----------------------------";
	LOG_TRACE << outputBuffer_.avail();
	LOG_TRACE << "-----------------------------";
	LOG_TRACE << "-----------------------------";
	int n = outputBuffer_.bufWriteFd(fd_, outputBuffer_.avail());
	if (n <= 0) {
		LOG_ERROR << "socket write error";
	}
	LOG_TRACE << "write " << n << "bytes";
	outputBuffer_.showContent();
	LOG_TRACE << outputBuffer_.avail();
	
	if (writeCompleteCallback_) {
		writeCompleteCallback_(writeCompleteArgs_, this);
	}
	if (outputBuffer_.avail() == 0) {
		channel_.dissetEvent(Channel::kWriteEvent);
		loop_->updateChannel(&channel_);
	}
}

void TcpConnection::handleClose()
{
	loop_->removeChannel(&channel_);
	::close(fd_);
	closeCallback_(closeArgs_, this);
}

void TcpConnection::handleError()
{
	LOG_ERROR << "socket error";
	errorCallback_(errorArgs_, this);
}
