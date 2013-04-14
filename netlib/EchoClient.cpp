#include "EchoClient.h"

using namespace mynet;

EchoClient::EchoClient(const char *name, const char *ipAddr, int port)
 :  rpcClient_(name, ipAddr, port)
{
	rpcClient_.start();
}

const char *EchoClient::echo(const char *str)
{
	return rpcClient_.echoStub(str);
}
