#ifndef CONNECTOR_H
#define CONNECTOR_H

namespace mynet {

class EventLoop;
class Channel;
class TcpConnection;

class Connector {
public:
	typedef void (*Callback)(void *args, TcpConnection *);
	Connector(EventLoop *, const char *, int, bool = true);
	~Connector();

	void setConnectionCallback(Callback, void *args);
	void start();
	void connecting();
	int fd();
private:
	static void handleWrite(void *args);
	void handleWrite();
	EventLoop *loop_;
	Channel *channel_;
	int fd_;
	char peerAddr_[50];
	int peerPort_;
	bool connected_;
	bool retry_;

	Callback connectionCallback_;
	void *connectionArgs_;
};
}
#endif
