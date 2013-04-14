#ifndef TCP_CONNECTION
#define TCP_CONNECTION

#include "Channel.h"
#include "Buffer.h"
namespace mynet {

class EventLoop;
class Channel;
class Buffer;

class TcpConnection {
public:
	TcpConnection(int sockfd, EventLoop *, const char *peerAddr, int peerPort);
	~TcpConnection();
	typedef void (*Callback)(void *, TcpConnection *);
	void setMessageCallback(Callback);
	void setCloseCallback(Callback);
	void setErrorCallback(Callback);
	void setWriteCompleteCallback(Callback);
	void setMessageArgs(void *args);
	void setCloseArgs(void *args);
	void setErrorArgs(void *args);
	void setWriteCompleteArgs(void *args);
	void sendMsg(void *, int);
	Buffer *getInputBuf();
	Buffer *getOutputBuf();
	int fd();
	const char *peerAddr();
	int peerPort();
private:
	int fd_;
	static void readCallback(void *);
	static void writeCallback(void *);
	static void errorCallback(void *);
	static void closeCallback(void *);
	int handleRead();
	void handleWrite();
	void handleClose();
	void handleError();
	EventLoop *loop_;
	Channel channel_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;
	Callback messageCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
	Callback writeCompleteCallback_;
	void *messageArgs_;
	void *closeArgs_;
	void *errorArgs_;
	void *writeCompleteArgs_;
	char peerAddr_[50];
	int peerPort_;
};

}
#endif
