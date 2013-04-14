#ifndef FSRPCCLIENT_H
#define FSRPCCLIENT_H

#include "../netlib/TcpBlockingClient.h"
#include "Dirent.h"
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
	int getattrStub(const char *path, struct stat *stbuf);
	int readdirStub(const char *path, Dirent **dirents);
	int mknodStub(const char *path, mode_t mode);
	int mkdirStub(const char *path, mode_t mode);
	int unlinkStub(const char *path);
	int rmdirStub(const char *path);
	int renameStub(const char *from, const char *to);
	int searchDirStub(const char *path);


	/* stubs to send message to chunkserver */
	int addChunkStub(long long chunkId);
	int delChunkStub(long long chunkId);
	int writeDataStub(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, string data);
	int writeStub(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, int offset);
	int readStub(long long chunkId, int offset, int length, string &data);
private:
	TcpBlockingClient tcpClient_;
};
}
#endif
