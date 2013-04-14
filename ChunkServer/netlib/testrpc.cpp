#include "RpcServer.h"
#include "EventLoop.h"
#include "base/Logging.h"
#include <cstdlib>
#include <cstring>
using namespace mynet;
void *echoMessageCallback(void *obj, const void *msg, int msg_len, int *resMsgLen)
{
	char *resMsg = (char *)malloc(msg_len * sizeof(char) + 4);
	LOG_INFO << (char *)msg << " " << msg_len;
	int type = 2;
	memcpy(resMsg, &type, 4);
	memcpy(resMsg + 4, msg, msg_len);
	*resMsgLen = msg_len + 4;
	LOG_INFO << (char *)resMsg << " " << *resMsgLen;
	return (void *)resMsg;
}

int main()
{
	EventLoop loop;
	RpcServer rpcServer(&loop, "rpc-server", "127.0.0.1", 5555, 10);

	rpcServer.registerRpc(1, echoMessageCallback, NULL);
	rpcServer.start();

	loop.loop();
	return 0;
}
