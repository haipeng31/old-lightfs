#include "FsRpcClient.h"
#include "FileSystem.pb.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include "netlib/base/Logging.h"
#include <ctime>

using mynet::Logger;
using namespace myfs;

FsRpcClient::FsRpcClient(const char *name, const char *ipAddr, int port)
 :  tcpClient_(name, ipAddr, port)
{
}

void FsRpcClient::start()
{
	tcpClient_.start();
}

void FsRpcClient::checkState()
{
	tcpClient_.checkState();
}

int FsRpcClient::addChunkStub(long long chunkId)
{
	/* generate request, and serialize to byte stream */
	AddChunkReq addChunkReq;
	addChunkReq.set_chunkid(chunkId);
	string reqByteStream;
	addChunkReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 17;
	reqByteStream.insert(0, (char *)&type, 4);

	/* send through socket */
	tcpClient_.sendMsg(reqByteStream.c_str(), reqByteStream.size());

	/* receive response from socket */
	void *repByteStream = NULL;
	int repByteStreamLen = 0;
	repByteStream = tcpClient_.recvMsg(&repByteStreamLen);

	/* get message type field */
	int repType;
	memcpy(&repType, repByteStream, 4);
	assert(repType == 18);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	AddChunkRep addChunkRep;
	addChunkRep.ParseFromString(repMsg);
	
	/* error occur */
	if (addChunkRep.returnval() < 0) {
		free((char *)repByteStream);
		return addChunkRep.returnval();
	}

	return 0;
}

int FsRpcClient::delChunkStub(long long chunkId)
{
	/* generate request, and serialize to byte stream */
	DelChunkReq delChunkReq;
	delChunkReq.set_chunkid(chunkId);
	string reqByteStream;
	delChunkReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 19;
	reqByteStream.insert(0, (char *)&type, 4);

	/* send through socket */
	tcpClient_.sendMsg(reqByteStream.c_str(), reqByteStream.size());

	/* receive response from socket */
	void *repByteStream = NULL;
	int repByteStreamLen = 0;
	repByteStream = tcpClient_.recvMsg(&repByteStreamLen);

	/* get message type field */
	int repType;
	memcpy(&repType, repByteStream, 4);
	assert(repType == 20);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	DelChunkRep delChunkRep;
	delChunkRep.ParseFromString(repMsg);
	
	/* error occur */
	if (delChunkRep.returnval() < 0) {
		free((char *)repByteStream);
		return delChunkRep.returnval();
	}

	return 0;
}


int FsRpcClient::writeDataStub(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, string data)
{
	/* generate request, and serialize to byte stream */
	WriteDataReq writeDataReq;

	for (int i = 0; i < csAddrs.size(); i++) {
		WriteDataReq_CSAddr *csAddr = writeDataReq.add_serveraddrs();
		csAddr->set_ipaddr(csAddrs[i].ipAddr());
		csAddr->set_port(csAddrs[i].port());
	}
	writeDataReq.set_chunkid(chunkId);
	writeDataReq.set_cacheid(cacheId);
	writeDataReq.set_data(data);
	string reqByteStream;
	writeDataReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 21;
	reqByteStream.insert(0, (char *)&type, 4);

	LOG_TRACE << "finish generating request";
	
	LOG_TRACE << reqByteStream.size();
	/* send through socket */
	tcpClient_.sendMsg(reqByteStream.c_str(), reqByteStream.size());
	
	LOG_TRACE << "finish sending message";
	/* receive response from socket */
	void *repByteStream = NULL;
	int repByteStreamLen = 0;
	repByteStream = tcpClient_.recvMsg(&repByteStreamLen);
	assert(repByteStream != NULL);
	LOG_TRACE << "finish receiving message";
	/* get message type field */
	int repType;
	memcpy(&repType, repByteStream, 4);
	assert(repType == 22);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	WriteDataRep writeDataRep;
	writeDataRep.ParseFromString(repMsg);
	
	LOG_TRACE << "finish receving response";
	/* error occur */
	if (writeDataRep.returnval() < 0) {
		free((char *)repByteStream);
		return writeDataRep.returnval();
	}

	return 0;
}

int FsRpcClient::writeStub(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, int offset)
{
	/* generate request, and serialize to byte stream */
	WriteReq writeReq;

	for (int i = 0; i < csAddrs.size(); i++) {
		WriteReq_CSAddr *csAddr = writeReq.add_serveraddrs();
		csAddr->set_ipaddr(csAddrs[i].ipAddr());
		csAddr->set_port(csAddrs[i].port());
	}
	writeReq.set_chunkid(chunkId);
	writeReq.set_cacheid(cacheId);
	writeReq.set_offset(offset);
	string reqByteStream;
	writeReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 23;
	reqByteStream.insert(0, (char *)&type, 4);
	
	/* send through socket */
	tcpClient_.sendMsg(reqByteStream.c_str(), reqByteStream.size());
	
	/* receive response from socket */
	void *repByteStream = NULL;
	int repByteStreamLen = 0;
	repByteStream = tcpClient_.recvMsg(&repByteStreamLen);
	assert(repByteStream != NULL);
	
	/* get message type field */
	int repType;
	memcpy(&repType, repByteStream, 4);
	assert(repType == 24);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	WriteDataRep writeDataRep;
	writeDataRep.ParseFromString(repMsg);
	
	LOG_TRACE << "finish receving response";
	/* error occur */
	if (writeDataRep.returnval() < 0) {
		free((char *)repByteStream);
		return writeDataRep.returnval();
	}

	return 0;
}
