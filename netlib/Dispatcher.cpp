#include "Dispatcher.h"
#include "base/Logging.h"
#include <cstdlib>
#include <cstring>
#include <assert.h>

using namespace mynet;

void Dispatcher::registerMsgCallback(int msgType, MessageCallback messageCallback, void *objArg)
{
	CallbackStruct *callbackStruct = (CallbackStruct *)malloc(sizeof(CallbackStruct));
	
	//MessageCallback testCallback = messageCallback;
	callbackStruct->messageCallback = messageCallback;
	callbackStruct->objArg = objArg;
	assert(messageCallbacks_.find(msgType) == messageCallbacks_.end());
	messageCallbacks_[msgType] = callbackStruct;
}

void *Dispatcher::dispatchMessage(void *objArg, const void *msg, int msgLen, int *repMsgLen)
{
	Dispatcher *dispatcher = (Dispatcher *)objArg;
	return dispatcher->dispatchMessage(msg, msgLen, repMsgLen);
}

void *Dispatcher::dispatchMessage(const void *msg, int msgLen, int *resMsgLen)
{
	int msgType = -1;
	memcpy(&msgType, msg, 4);
	LOG_INFO << msgType;
	msg = (void *)((char *)msg + 4);
	msgLen -= 4;
	LOG_INFO << (char *)msg;
	CallbackStruct *callbackStruct = messageCallbacks_[msgType];

	return callbackStruct->messageCallback(callbackStruct->objArg, msg, msgLen, resMsgLen);
}
