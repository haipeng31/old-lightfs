#include "Codec.h"
//#include "Buffer.h"
#include "TcpConnection.h"
#include "base/Logging.h"
#include "base/ThreadPool.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
using namespace mynet;

const int Codec::kLengthSize = 4;

Codec::Codec(int lengthSize, ThreadPool *threadPool)
 :  lengthSize_(lengthSize),
	threadPool_(threadPool),
	sendLock_()
{
}

void Codec::setMessageCallback(MessageCallback messageCallback, void *objArg)
{
	messageCallback_ = messageCallback;
	objArg_ = objArg;
}
void Codec::dealMsg(void *objArg, TcpConnection *connection)
{
	Codec *codec = (Codec *)objArg;
	codec->dealMsg(connection);
}

void Codec::taskFunc(void *objArg, void *arg)
{
	Codec *codec = (Codec *)objArg;
	TaskFuncArg *taskFuncArg = (TaskFuncArg *)arg;

	void *resMsg = NULL;
	int resLen = 0;

	resMsg = codec->messageCallback_(codec->objArg_, taskFuncArg->msg, taskFuncArg->length, &resLen);
	delete (char *)taskFuncArg->msg;
	assert(resLen != 0);
	int type;
	memcpy(&type, resMsg, 4);
	LOG_TRACE << "----------------Type " << type;

	resMsg = codec->encode(resMsg, &resLen);
	assert(resLen != 0);
	{
		MutexLockGuard lockGuard(codec->sendLock_);
		LOG_TRACE << "In thread func " << resLen;
		int type;
		memcpy(&type, (char *)resMsg + 4, 4);
		LOG_TRACE << "----------------Type " << *(int *)((char *)resMsg + 4);
		taskFuncArg->con->sendMsg(resMsg, resLen);
	}
}

void Codec::dealMsg(TcpConnection *connection)
{
	int length = -1;
	Buffer *buf = connection->getInputBuf();
	LOG_INFO << buf->avail() << " " << lengthSize_;
	while (buf->avail() >= lengthSize_) {
		buf->readBuf((char *)&length, lengthSize_);
		
		LOG_INFO << length;
		if (length >= buf->avail()) {
			char *msg = (char *)malloc(length);
			memset(msg, 0, length);
			buf->readBuf(msg, length);

			/* when using thread_pool, put the following in the thread_Pool 
			char *resMsg = NULL;
			int resLen = 0;

		* the callback should delete msg and malloc resMsg
			 * we don't prepare 4bytes for the lenth now, fix it
			 * better to make all these in Bufer class
			 *
			resMsg = (char *)messageCallback_(objArg_, (const void *)msg, length, &resLen);
			delete msg;

			resMsg = (char *)encode(resMsg, &resLen);
			connection->sendMsg(resMsg, resLen);
			*/
			
			TaskFuncArg *taskFuncArg = new TaskFuncArg;
			taskFuncArg->con = connection;
			taskFuncArg->msg = (void *)msg;
			taskFuncArg->length = length;
			Task *task = new Task;
			task->taskFunc = taskFunc;
			task->objArg = (void *)this;
			task->args = taskFuncArg;
			

			/* put the write/append request to the write thread */
			int type = -1;
			memcpy(&type, msg, 4);
			if (type == 3) {			// the request is write/append requet
				threadPool_->putWriteTask(task);
			} else {
				threadPool_->putTask(task);
			}
		} else {
			break;
		}
	}
}


void *Codec::encode(void *resMsg, int *resLen)
{
	char *msg = (char *)malloc(*resLen + lengthSize_);
	memcpy(msg, resLen, lengthSize_);
	memcpy(msg + lengthSize_, resMsg, *resLen);
	free(resMsg);
	*resLen += 4;
	return msg;
}
