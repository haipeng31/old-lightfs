#ifndef METASERVER_H
#define METASERVER_H

#include "netlib/EventLoop.h"
#include "netlib/RpcServer.h"
#include "MetaDataManager.h"
#include "Client/Dirent.h"
#include <sys/stat.h>
#include "INode.h"
using mynet::EventLoop;
using mynet::RpcServer;

namespace myfs {
class MetaServer {
public:
	MetaServer(const char *name, const char *ipAddr, int port, int nthread);
	void start();
	

	static void *getattrCallback(void *, const void *, int, int *);
	static void *readdirCallback(void *, const void *, int, int *);
	static void *mknodCallback(void *, const void *, int, int *);
	static void *mkdirCallback(void *, const void *, int, int *);
	static void *unlinkCallback(void *, const void *, int, int *);
	static void *rmdirCallback(void *, const void *, int, int *);
	static void *renameCallback(void *, const void *, int, int *);
	static void *searchDirCallback(void *, const void *, int, int *);

private:
	void *getattrCallback(const void *, int, int *);
	void *readdirCallback(const void *, int, int *);
	void *mknodCallback(const void *, int, int *);
	void *mkdirCallback(const void *, int, int *);
	void *unlinkCallback(const void *, int, int *);
	void *rmdirCallback(const void *, int, int *);
	void *renameCallback(const void *, int, int *);
	void *searchDirCallback(const void *, int, int *);

	string pathToName(string path);
/* precedures to be called by remote client */
	int getattr(const char *path, struct stat *stbuf);
	int readdir(const char *path, Dirent **dirents);
	int mknod(const char *path, mode_t mode);
	int mkdir(const char *path, mode_t mode);
	int unlink(const char *path);
	int rmdir(const char *path);
	int rename(const char *from, const char *to);
	int searchDir(const char *path);
	EventLoop loop_;
	RpcServer rpcServer_;
	MetaDataManager metaDataManager_;
};
}
#endif
