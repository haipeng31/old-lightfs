#ifndef RPCCLIENT_H
#define RPCCLIENT_H
#include "TcpBlockingClient.h"
namespace mynet {
class RpcClient {
public:
	RpcClient(const char *, const char *, int);
	void start();
	const char *echoStub(const char *);
private:
	TcpBlockingClient tcpClient_;
};
}
#endif
