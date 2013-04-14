#include "RpcClient.h"
#include <cstdlib>
#include <cstring>
#include "base/Logging.h"

using namespace mynet;

RpcClient::RpcClient(const char *name, const char *ipAddr, int port)
 :  tcpClient_(name, ipAddr, port)
{
	
}

void RpcClient::start()
{
	tcpClient_.start();
}

const char *RpcClient::echoStub(const char *str)
{
	int msgType = 1;

	/* using google protobuf's discrete message type object
	 * to get the marshalled data.
	 * In this test, we directly use the str
	 */
	char byteStream[20];
	memset(byteStream, 0, sizeof(byteStream));
	strncpy(byteStream, str, strlen(str));

	memcpy(byteStream + 4, byteStream, strlen(byteStream));
	memcpy(byteStream, &msgType, 4);

	tcpClient_.sendMsg(byteStream, strlen(str) + 4);

	char *recvMsg, *pData;
	int recvLen;
	
	LOG_INFO << "BEFORE RECVEIV";
	recvMsg = (char *)tcpClient_.recvMsg(&recvLen);
	LOG_INFO << "AFTER RECEIVE";
	int recvType;
	memcpy(&recvType, recvMsg, 4);
	
	/* using google protobuf to unmarshall received data */
	char *result = (char *)malloc(20);
	memset(result, 0, 20);
	strncpy(result, recvMsg + 4, recvLen - 4);
	free(recvMsg);

	return result;
}
