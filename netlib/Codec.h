#ifndef CODEC_H
#define CODEC_H

#include "base/Mutex.h"
namespace mynet {

class TcpConnection;
class ThreadPool;

struct TaskFuncArg {
	TcpConnection *con;
	const void *msg;
	int length;
};

class Codec {
public:
	typedef void *(*MessageCallback)(void *objArg, const void *, int, int *);
	Codec(int lengthSize = kLengthSize, ThreadPool *threadPool = NULL);
	
	static void dealMsg(void *, TcpConnection *);
	static void taskFunc(void *objArg, void *arg);	
	void setMessageCallback(MessageCallback messageCallback, void *objArg);
private:
	void dealMsg(TcpConnection *connection);
	void *encode(void *, int *);
	void encode(void *data, int len);
	static const int kLengthSize;
	int lengthSize_;
	MessageCallback messageCallback_;
	void *objArg_;

	ThreadPool *threadPool_;
	MutexLock sendLock_;
};
}
#endif
