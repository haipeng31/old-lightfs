#ifndef TCPCLIENT_H
#define TCPCLIENT_H

namespace mynet {

class EventLoop;
class TcpConnection;
class Connector;

class TcpClient {
public:
	typedef void (*Callback)(void *args, TcpConnection *);
	TcpClient(EventLoop *, const char *, int, bool = true);
	~TcpClient();
	void setConnectCallback(Callback, void *);
	void setMessageCallback(Callback, void *);
	void setCloseCallback(Callback, void *);
	void setErrorCallback(Callback, void *);
	void setWriteCompleteCallback(Callback, void *);
	void start();
private:
	static void handleWrite(void *args, TcpConnection *);
	static void handleClose(void *args, TcpConnection *);
	void handleWrite(TcpConnection *);
	void handleClose(TcpConnection *);
	EventLoop *loop_;
	Connector *connector_;
	TcpConnection *connection_;
	int fd_;
	bool retry_;
	bool connected_;
	char peerAddr_[50];
	int peerPort_;

	Callback connectCallback_;
	Callback messageCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
	Callback writeCompleteCallback_;
	void *connectArgs_;
	void *messageArgs_;
	void *closeArgs_;
	void *errorArgs_;
	void *writeCompleteArgs_;
};

}
#endif
