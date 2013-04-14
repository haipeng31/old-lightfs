#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <map>

using std::map;
namespace mynet {

class EventLoop;
class Acceptor;
class TcpConnection;

class TcpServer {
public:
	typedef void (*MessageCallback)(void *arg, TcpConnection *);
	typedef void (*ConnectionCallback)(void *arg, TcpConnection *);
	typedef void (*CloseCallback)(void *arg, TcpConnection *);
	TcpServer(EventLoop *loop, const char *, const char *, int);
	~TcpServer();
	void start();
	void setMessageCallback(MessageCallback, void *);
	void setConnectionCallback(ConnectionCallback, void *);
	void setCloseCallback(ConnectionCallback, void *);
private:
	typedef map<int, TcpConnection *> ConnectionMap;

	static void handleNewConnection(void *args, TcpConnection *);
	void handleNewConnection(TcpConnection *);
	static void handleClose(void *args, TcpConnection *);
	void handleClose(TcpConnection *);
	EventLoop *loop_;
	Acceptor *acceptor_;	
	ConnectionMap connections_;
	char name_[50];
	char ipAddr_[20];
	int port_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
	CloseCallback closeCallback_;
	void *messageArgs_;
	void *connectionArgs_;
	void *closeArgs_;
	bool started_;
};
}
#endif
