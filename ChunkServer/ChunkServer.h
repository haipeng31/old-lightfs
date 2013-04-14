#ifndef CHUNKSERVER_H
#define CHUNKSERVER_H

#include "netlib/EventLoop.h"
#include "netlib/RpcServer.h"
#include "ChunkManager.h"
#include "LeaseManager.h"
#include "CacheDataManager.h"
#include "CSConManager.h"

#include <string>

using mynet::EventLoop;
using mynet::RpcServer;
using std::string;
namespace myfs {
class ChunkServer {

public:
	ChunkServer(const char *name, const char *ipAddr, int port, int nthread, int nwriteThread, string dir, int maxchunk);
	void start();
	~ChunkServer();

	/* rpc callback functions */
	static void *addChunkCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen);
	static void *delChunkCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen);
	static void *writeDataCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen);
	static void *writeCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen);
	static void *readCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen);
private:
	void *addChunkCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen);
	void *delChunkCallback(const void *reqMsg, int reqMsgLen, int * repMsgLen);
	void *writeDataCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen);
	void *writeCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen);
	void *readCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen);
	EventLoop loop_;
	RpcServer rpcServer_;
	ChunkManager chunkManager_;
	LeaseManager leaseManager_;
	CacheDataManager cacheDataManager_;
	CSConManager cSConManager_;
};
}
#endif
