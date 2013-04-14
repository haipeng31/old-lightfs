#ifndef ECHOCLIENT_H
#define ECHOCLIENT_H

#include "RpcClient.h"
namespace mynet {
class EchoClient {
public:
	EchoClient(const char *name, const char *ipAddr, int port);
	const char *echo(const char *);

private:
	RpcClient rpcClient_;
};
}
#endif
