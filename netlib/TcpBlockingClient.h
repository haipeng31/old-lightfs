#ifndef TCPBLOCKINGCLIENT_H
#define TCPBLOCKINGCLIENT_H

#include "Buffer.h"
#include "base/Mutex.h"
namespace mynet {
class TcpBlockingClient {

public:
	TcpBlockingClient(const char *,const char *, int);
	void start();
	void sendMsg(const void *, int);
	void *recvMsg(int *);
private:
	static const int lengthSize_;
	char name_[50];
	char serverAddr_[20];
	int serverPort_;
	int sockfd_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;
	bool connected_;
	MutexLock sendLock_;
	MutexLock recvLock_;
};
}
#endif
