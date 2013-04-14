#ifndef RPCSERVER_H
#define RPCSERVER_H

#include "Codec.h"
#include "Dispatcher.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "base/ThreadPool.h"

namespace mynet {

/* using registerRpc to register rpc before calling start()
 * function
 */
class RpcServer {
public:
	typedef void *(*MessageCallback)(void *, const void *, int, int *);
	RpcServer(EventLoop *, const char *, const char *, int, int, int);
	~RpcServer();

	void start();
	void registerRpc(int, MessageCallback, void *); 
private:
	static void onConnection(void *objArg, TcpConnection *);
	static void onClose(void *objArg, TcpConnection *);
	static void onError(void *objArg, TcpConnection *);
	
	ThreadPool threadPool_;
	TcpServer server_;
	Codec codec_;
	Dispatcher dispatcher_;
};
}
#endif
