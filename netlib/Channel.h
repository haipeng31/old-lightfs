#ifndef CHANNEL_H
#define CHANNEL_H

namespace mynet {

class Channel {
public:
	typedef void (*Callback)(void *);
	static const int kReadEvent;
	static const int kWriteEvent;
	static const int kNoEvent;
	Channel(int fd);
	void setEvent(int);
	void dissetEvent(int);
	int getEvent();
	void setRevent(int);
	void handleEvent();
	void setReadCallback(Callback);
	void setWriteCallback(Callback);
	void setCloseCallback(Callback);
	void setErrorCallback(Callback);
	void setArgs(void *args);
	void *args();
	int fd();
private:
	int fd_;
	int events_;
	int revents_;
	Callback readCallback_;
	Callback writeCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
	void *args_;
};

}
#endif
