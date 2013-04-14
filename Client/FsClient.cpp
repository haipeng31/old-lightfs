#include "FsClient.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

using namespace myfs;

long long FsClient::id_ = 1;

FsClient::FsClient(const char *name, const char *ipAddr, int port)
 :  fsRpcClient_(name, ipAddr, port)
{
	fsRpcClient_.start();
}

int FsClient::getattr(const char *path, struct stat *stbuf)
{
	/* rpc call */
	return fsRpcClient_.getattrStub(path, stbuf);
}

int FsClient::readdir(const char *path, Dirent **dirents)
{
	return fsRpcClient_.readdirStub(path, dirents);
}

int FsClient::mknod(const char *path, mode_t mode)
{
	return fsRpcClient_.mknodStub(path, mode);
}

int FsClient::mkdir(const char *path, mode_t mode)
{
	return fsRpcClient_.mkdirStub(path, mode);
}

int FsClient::unlink(const char *path)
{
	return fsRpcClient_.unlinkStub(path);
}

int FsClient::rmdir(const char *path)
{
	return fsRpcClient_.rmdirStub(path);
}

int FsClient::rename(const char *from, const char *to)
{
	return fsRpcClient_.renameStub(from, to);
}

int FsClient::opendir(const char *path)
{
	return fsRpcClient_.searchDirStub(path);
}
int FsClient::open(const char *path)
{
#if 0
	/* chenk whether file exist */
	bool exist = fsRpcClient_.existStub(path);
	if (!exist) {
		/* chenk error info to do return ,fix me */
		return -1;
	}
	
	/* check mode to check permission, not implement */

	/* generate the file handle for the file */
	uint64_t fd = genId();
	return fd;
#endif
	return 0;
}

int FsClient::read(uint64_t fd, const char *path, char *buf, uint32_t size, uint32_t off)
{
#if 0
	int fileSize = fsRpcClient_.getSizeStub(path);
	int nread = 0;
	bool eof = false;
	while (!eof && size) {
		int index = off / kChunkSize;
		int left = (index + 1) * kChunkSize - off;
		int len = left < size ? left : size;
		
		if (off + len > fileSize) {
			len = fileSize - off;
			eof = true;
		}
		/* see whether the chunk server infos are buffered */
		if ((indexs_.find(fd) != indexs_.end()) && (indexs_[fd] != index)) {
			/* delete the chunk server infos in buufer */

			/* disconnect the chunkRpcClient */
		}

		if (indexs_.find(fd) == indexs_.end()) {
			/* ask the meta server the chunk server infos */


			/* store the chunk server infos in buffer */

			/* create chunkRpcClient and connect to chunkserver */

		}
		
		ChunkRpcClient *chunkRpcClient = chunkRpcClients_[fd];
		chunkRpcClient->readStub(chunkName, buf, len, off - index * kChunkSize);


		size -= len;
		off += len;
		
	}
#endif
	return 0;
}

int FsClient::write(uint64_t fd, const char *path, const char *buf, uint32_t size, uint32_t off)
{
	return 0;
}

int FsClient::release(uint64_t fd, const char *path)
{
#if 0
	if (chunkInfoBuffers_.find(fd) != chunkInfoBuffers_.end()) {
		chunkInfoBuffers_.remove(fd);
	}

	if (indexs_.find(fd) != indexs_.end()) {
		indexs_.remove(fd);
	}
#endif
	return 0;
}


long long FsClient::genFileId()
{
	return id_++;
}


int FsClient::addChunk(long long chunkId)
{
	return fsRpcClient_.addChunkStub(chunkId);
}

int FsClient::delChunk(long long chunkId)
{
	return fsRpcClient_.delChunkStub(chunkId);
}

int FsClient::writeData(vector<ChunkServerAddr> csAddrs, long long chunkid, int cacheid, string data)
{
	return fsRpcClient_.writeDataStub(csAddrs, chunkid, cacheid, data);
}

int FsClient::write(vector<ChunkServerAddr> csAddrs, long long chunkId, int cacheId, int offset)
{
	return fsRpcClient_.writeStub(csAddrs, chunkId, cacheId, offset);
}

int FsClient::read(long long chunkId, int offset, int length, string &data)
{
	return fsRpcClient_.readStub(chunkId, offset, length, data);
}

