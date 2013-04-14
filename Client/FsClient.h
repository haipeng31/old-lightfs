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
	int getattr(const char *path, struct stat *stbuf);

	/* need to define Dirent in metadata server ,it contain file name and file type,
	 * return number of dirent or negetive value if error
	 */
	int readdir(const char *path, Dirent **dirents); 
	
	int mknod(const char *path, mode_t mode);
	int mkdir(const char *path, mode_t mode);
	int unlink(const char *path);
	int rmdir(const char *path);
	int rename(const char *from, const char *to);
	int opendir(const char *path);	
	/* return the generated id if susseed or negetive value if error */
	int open(const char *path);
	int read(uint64_t fd, const char *path, char *buf, uint32_t size, uint32_t off);
	int write(uint64_t fd, const char *path, const char *buf, uint32_t size, uint32_t off);
	int release(uint64_t fd, const char *path);	
	static long long genFileId();


	/* chunkserver rpc calls */
	int addChunk(long long chunkId);
	int delChunk(long long chunkId);
	int writeData(vector<ChunkServerAddr> csAddrs, long long chunkid, int cacheid, string data);
	int write(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, int offset);
	int read(long long chunkId, int offset, int length, string &data);
private:
#if 0
	map<long long, unsigned int> filePointers_;
	map<long long, ChunkInfo> chunkInfoBuffers_;
	map<long long, ChunkRpcClient *> chunkRpcClients_;
	/* used to decide whether ChunkInfo is in buffer */
	map<long long, unsigned int> indexs_;
#endif
	FsRpcClient fsRpcClient_;

	static long long id_;
	static const uint32_t kChunkSize = 4 * 1024;

};
}
#endif
