#ifndef FSRPCCLIENT_H
#define FSRPCCLIENT_H

#include "netlib/TcpBlockingClient.h"
#include "CSConManager.h"          // put ChunkServerAddr as a new class
#include <sys/stat.h>
#include <string>
#include <vector>
using std::string;
using std::vector;
using mynet::TcpBlockingClient;
namespace myfs {
class FsRpcClient {
public:
	FsRpcClient(const char *, const char *, int);
	void start();
	
	void checkState();
	/* stubs to send message to chunkserver */
	int addChunkStub(long long chunkId);
	int delChunkStub(long long chunkId);
	int writeDataStub(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, string data);
	int writeStub(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, int offset);
private:
	TcpBlockingClient tcpClient_;
};
}
#endif
