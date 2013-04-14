#ifndef FSCLIENT_H
#define FSCLIENT_H

#include <stdint.h>
#include <map>
#include <vector>
#include "FsRpcClient.h"
using std::map;
using std::vector;

namespace myfs {
class FsClient {
public:
	FsClient(const char *, const char *, int);
	
	void checkState();
	/* chunkserver rpc calls */
	int addChunk(long long chunkId);
	int delChunk(long long chunkId);
	int writeData(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, string data);
	int write(vector<ChunkServerAddr> csAddrs, long long chunkiId, int cacheId, int offset);
private:
#if 0
	map<long long, unsigned int> filePointers_;
	map<long long, ChunkInfo> chunkInfoBuffers_;
	map<long long, ChunkRpcClient *> chunkRpcClients_;
	/* used to decide whether ChunkInfo is in buffer */
	map<long long, unsigned int> indexs_;
#endif
	FsRpcClient fsRpcClient_;


};
}
#endif
