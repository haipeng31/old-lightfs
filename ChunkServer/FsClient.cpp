#include "FsClient.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

using namespace myfs;


FsClient::FsClient(const char *name, const char *ipAddr, int port)
 :  fsRpcClient_(name, ipAddr, port)
{
	fsRpcClient_.start();
}

void FsClient::checkState()
{
	fsRpcClient_.checkState();
}

int FsClient::addChunk(long long chunkId)
{
	return fsRpcClient_.addChunkStub(chunkId);
}

int FsClient::delChunk(long long chunkId)
{
	return fsRpcClient_.delChunkStub(chunkId);
}

int FsClient::writeData(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, string data)
{
	return fsRpcClient_.writeDataStub(csAddrs, chunkId, cacheId, data);
}

int FsClient::write(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, int offset)
{
	return fsRpcClient_.writeStub(csAddrs, chunkId, cacheId, offset);
}
