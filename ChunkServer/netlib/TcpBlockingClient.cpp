#include "TcpBlockingClient.h"
#include "SockOps.h"
#include "base/Logging.h"

#include <cstdlib>
#include <cstring>
#include <assert.h>

using namespace mynet;

const int TcpBlockingClient::lengthSize_ = 4;

TcpBlockingClient::TcpBlockingClient(const char *name, const char *ipAddr, int port)
 :	inputBuffer_(128),
	outputBuffer_(128),
	sendLock_(),
	recvLock_()
{
	memset(name_, 0, sizeof(name_));
	memset(serverAddr_, 0, sizeof(serverAddr_));
	serverPort_ = 0;

	strncpy(name_, name, strlen(name));
	strncpy(serverAddr_, ipAddr, strlen(ipAddr));
	serverPort_ = port;

	sockfd_ = socket();
	connected_ = false;
}

void TcpBlockingClient::start()
{
	assert(connected_ == false);
	int rt = connect(sockfd_, serverAddr_, serverPort_);
	if (rt < 0) {
		LOG_ERROR << "Connect to server fail";
	}
	
	connected_ = true;
}

void TcpBlockingClient::checkState()
{
	inputBuffer_.checkState();
	outputBuffer_.checkState();
}

/* The len field is added to the msg */
void TcpBlockingClient::sendMsg(const void *msg, int msgLen)
{
	//MutexLockGuard lock(sendLock_);
	assert(connected_ == true);

	LOG_TRACE << "Write to Buffer";
	outputBuffer_.writeBuf((const char *)&msgLen, lengthSize_);
	LOG_TRACE << "Write to buffer len" << msgLen;
	outputBuffer_.writeBuf((const char *)msg, msgLen);
	LOG_TRACE << "Write to buffer msg";
	LOG_TRACE << "Write to Buffer success";
	LOG_TRACE << "Write to Socket";
	while (outputBuffer_.avail()) {
		outputBuffer_.bufWriteFd(sockfd_, outputBuffer_.avail());
		LOG_TRACE << "Wrote to Socket";
	}
}

void *TcpBlockingClient::recvMsg(int *msgLen)
{
	MutexLockGuard lock(recvLock_);
	assert(connected_ == true);
	
	
	char *recvMsg = NULL;
	int len = -1;
	while (!recvMsg) {
		LOG_INFO << "recvMsg";
		inputBuffer_.bufReadFd(sockfd_);
		if ((len == -1) && (inputBuffer_.avail() >= lengthSize_)) {
			inputBuffer_.readBuf((char *)&len, 4);
			LOG_INFO << len;
		}

		if ((len != -1) && (inputBuffer_.avail() >= len)) {
			recvMsg = (char *)malloc(len);
			memset(recvMsg, 0, len);
			inputBuffer_.readBuf(recvMsg, len);
			*msgLen = len;
		}
	}
	return recvMsg;
	
}
