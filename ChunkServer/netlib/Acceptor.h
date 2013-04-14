#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"

namespace mynet {

class TcpConnection;
class EventLoop;
class Acceptor {
public:
	typedef void (*Callback)(void *, TcpConnection *);
	Acceptor(EventLoop *, const char *, int);
	~Acceptor();
	void start();
	void setConnectionCallback(Callback, void *args);
private:
	static void readCallback(void *);
	void handleRead();
	char ipAddr_[20];
	int port_;
	EventLoop *loop_;
	int listenfd_;
	Channel channel_;
	Callback connectionCallback_;
	void *connectionArgs_;
	bool started_;
};
}
#endif
