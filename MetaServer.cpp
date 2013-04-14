#include "MetaServer.h"
#include "Client/FileSystem.pb.h"
#include <string>
#include <errno.h>
#include "netlib/base/Logging.h"
#include <ctime>
using std::string;
using mynet::Logger;
using namespace myfs;

MetaServer::MetaServer(const char *name, const char *ipAddr, int port, int nthread)
 :  loop_(),
	rpcServer_(&loop_, name, ipAddr, port, nthread)
{
	/* register rpc calls here */
	rpcServer_.registerRpc(1, getattrCallback, this);
	rpcServer_.registerRpc(3, readdirCallback, this);
	rpcServer_.registerRpc(5, mknodCallback, this);
	rpcServer_.registerRpc(7, mkdirCallback, this);
	rpcServer_.registerRpc(9, unlinkCallback, this);
	rpcServer_.registerRpc(11, rmdirCallback, this);
	rpcServer_.registerRpc(13, renameCallback, this);
	rpcServer_.registerRpc(15, searchDirCallback, this);
}

void MetaServer::start()
{
	rpcServer_.start();
	loop_.loop();
}

void *MetaServer::getattrCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->getattrCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::readdirCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->readdirCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::mknodCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->mknodCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::mkdirCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->mkdirCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::unlinkCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->unlinkCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::rmdirCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->rmdirCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::renameCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->renameCallback(reqMsg, reqMsgLen, repMsgLen); 
}

void *MetaServer::searchDirCallback(void *obj, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	MetaServer *metaServer = (MetaServer *)obj;
	return metaServer->searchDirCallback(reqMsg, reqMsgLen, repMsgLen);
}

void *MetaServer::getattrCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	GetAttrReq getAttrReq;
	getAttrReq.ParseFromString(reqByteStream);
	
	struct stat stbuf;
	int rt = getattr(getAttrReq.path().c_str(), &stbuf);
	
	LOG_INFO << "mode = " << std::oct << stbuf.st_mode << std::dec;
	LOG_INFO << "uid = " << stbuf.st_uid;
	LOG_INFO << "gid = " << stbuf.st_gid;
	LOG_INFO << "size = " << stbuf.st_size;
	LOG_INFO << "ctime = " << ctime(&stbuf.st_ctime);
	LOG_INFO << "atime = " << ctime(&stbuf.st_atime);
	LOG_INFO << "mtime = " << ctime(&stbuf.st_mtime);
	GetAttrRep getAttrRep;
	GetAttrRep_Stat *st = getAttrRep.mutable_st();
	st->set_mode(stbuf.st_mode);
	st->set_uid(stbuf.st_uid);
	st->set_gid(stbuf.st_gid);
	st->set_size(stbuf.st_size);
	st->set_atime(stbuf.st_atime);
	st->set_mtime(stbuf.st_mtime);
	st->set_ctime(stbuf.st_ctime);
	getAttrRep.set_returnval(rt);
	
	LOG_INFO << "mode = " << getAttrRep.st().mode();
	time_t atime = getAttrRep.st().atime();
	time_t ctimet = getAttrRep.st().ctime();
	time_t mtime = getAttrRep.st().mtime();
	LOG_INFO << "atime = " << ctime(&atime);
	LOG_INFO << "ctime = " << ctime(&ctimet);
	LOG_INFO << "mtime = " << ctime(&mtime);
	string repByteStream;
	getAttrRep.SerializeToString(&repByteStream);
	int type = 2;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();
	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//return (void *)repByteStream.c_str();
}

void *MetaServer::readdirCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	ReadDirReq readDirReq;
	readDirReq.ParseFromString(reqByteStream);
	
	Dirent *dirents;
	int size = readdir(readDirReq.path().c_str(), &dirents);
	
	ReadDirRep readDirRep;

	for (int i = 0; i < size; i++) {
		ReadDirRep_DirEnt *dirent = readDirRep.add_dirents();
		dirent->set_name(dirents[i].name);
		dirent->set_type(dirents[i].type);
	}
	delete [] dirents;
	readDirRep.set_returnval(size);

	string repByteStream;
	readDirRep.SerializeToString(&repByteStream);
	int type = 4;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();
	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//return (void *)repByteStream.c_str();
}

void *MetaServer::mknodCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	MkNodReq mkNodReq;
	mkNodReq.ParseFromString(reqByteStream);
	
	int rt = mknod(mkNodReq.path().c_str(), mkNodReq.mode());	
	
	MkNodRep mkNodRep;
	mkNodRep.set_returnval(rt);

	string repByteStream;
	mkNodRep.SerializeToString(&repByteStream);
	int type = 6;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//	return (void *)repByteStream.c_str();
}


void *MetaServer::mkdirCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	MkDirReq mkDirReq;
	mkDirReq.ParseFromString(reqByteStream);
	
	int rt = mkdir(mkDirReq.path().c_str(), mkDirReq.mode());	
	
	MkDirRep mkDirRep;
	mkDirRep.set_returnval(rt);

	string repByteStream;
	mkDirRep.SerializeToString(&repByteStream);
	int type = 8;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//	return (void *)repByteStream.c_str();
}

void *MetaServer::unlinkCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	UnlinkReq unlinkReq;
	unlinkReq.ParseFromString(reqByteStream);
	
	int rt = unlink(unlinkReq.path().c_str());	
	
	UnlinkRep unlinkRep;
	unlinkRep.set_returnval(rt);

	string repByteStream;
	unlinkRep.SerializeToString(&repByteStream);
	int type = 10;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//return (void *)repByteStream.c_str();
}

void *MetaServer::rmdirCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	RmDirReq rmDirReq;
	rmDirReq.ParseFromString(reqByteStream);
	
	int rt = rmdir(rmDirReq.path().c_str());	
	
	RmDirRep rmDirRep;
	rmDirRep.set_returnval(rt);

	string repByteStream;
	rmDirRep.SerializeToString(&repByteStream);
	int type = 12;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();
	
	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//	return (void *)repByteStream.c_str();
}

void *MetaServer::renameCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	return NULL;
}

void *MetaServer::searchDirCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	SearchDirReq searchDirReq;
	searchDirReq.ParseFromString(reqByteStream);
	
	int rt = searchDir(searchDirReq.path().c_str());	
	
	LOG_TRACE << "rt == " << rt;
	SearchDirRep searchDirRep;
	searchDirRep.set_returnval(rt);

	string repByteStream;
	searchDirRep.SerializeToString(&repByteStream);
	int type = 16;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();
	
	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
	//	return (void *)repByteStream.c_str();
}

string MetaServer::pathToName(string path)
{
	
	int pos = path.find_last_of('/');
	if (pos == (path.size() - 1)) {
		return "/";
	} else {
		return path.substr(pos + 1);
	}
}

int MetaServer::getattr(const char *path, struct stat *stbuf)
{
	INode *inode = metaDataManager_.getINode(path);
	if (inode == NULL) {
		return -ENOENT;
	} else {
		stbuf->st_mode = inode->mode();
		stbuf->st_uid = inode->uid();
		stbuf->st_gid = inode->gid();
		stbuf->st_size = inode->size();
		stbuf->st_atime = inode->atime();
		stbuf->st_mtime = inode->mtime();
		stbuf->st_ctime = inode->ctime();
		LOG_INFO << "path = " << path;
		LOG_INFO << "mode = " << std::oct << stbuf->st_mode << std::dec;
	LOG_INFO << "uid = " << stbuf->st_uid;
	LOG_INFO << "gid = " << stbuf->st_gid;
	LOG_INFO << "size = " << stbuf->st_size;
	LOG_INFO << "ctime = " << ctime(&stbuf->st_ctime);
	LOG_INFO << "atime = " << ctime(&stbuf->st_atime);
	LOG_INFO << "mtime = " << ctime(&stbuf->st_mtime);

		return 0;
	}
}

int MetaServer::readdir(const char *path, Dirent **dirents)
{
	list<INode *> inodes = metaDataManager_.readDir(path);
	LOG_TRACE << "readDir success";
	(*dirents) = new Dirent[inodes.size()];
	assert((*dirents) != NULL);
	LOG_TRACE << "malloc success";
	list<INode *>::iterator lit;
	int i = 0;

	for (lit = inodes.begin(); lit != inodes.end(); lit++) {
	//	LOG_TRACE << "TEST";
	//	string temp = (*lit)->key();
	//	string temp((*lit)->key().c_str());
		(*dirents)[i].name = pathToName((*lit)->key());
		(*dirents)[i].type = (*lit)->fileType();
		i++;
	//	LOG_TRACE << "TEST";
	}
	LOG_TRACE << "SUCEESS";
	return inodes.size();
}

int MetaServer::mknod(const char *path, mode_t mode)
{
	string pathStr(path);
	int pos = pathStr.find_last_of('/');
	string dirName = pathStr.substr(0, pos);
	if (dirName == "") {
		dirName = "/";
	}
	string fileName = pathStr.substr(pos + 1);

	int rt = metaDataManager_.addFile(dirName, fileName);
	if (rt == -2) {
		return -ENOENT;
	} else if (rt == -1) {
		return -EEXIST;
	} else {
		return 0;
	}
}

int MetaServer::mkdir(const char *path, mode_t mode)
{
	string pathStr(path);
	int pos = pathStr.find_last_of('/');
	string pdirName = pathStr.substr(0, pos);
	if (pdirName == "") {
		pdirName = "/";
	}
	string dirName = pathStr.substr(pos + 1);

	int rt = metaDataManager_.addDir(pdirName, dirName);
	if (rt == -2) {
		return -ENOENT;
	} else if (rt == -1) {
		return -EEXIST;
	} else {
		return 0;
	}
}

int MetaServer::unlink(const char *path)
{
	string pathStr(path);
	int pos = pathStr.find_last_of('/');
	string dirName = pathStr.substr(0, pos);
	if (dirName == "") {
		dirName = "/";
	}
	string fileName = pathStr.substr(pos + 1);

	int rt = metaDataManager_.delFile(dirName, fileName);
	if (rt == -2) {
		return -ENOTDIR;
	} else if (rt == -1) {
		return -ENOENT;
	} else {
		return 0;
	}
}

int MetaServer::rmdir(const char *path)
{
	string pathStr(path);
	int pos = pathStr.find_last_of('/');

	string pdirName = pathStr.substr(0, pos);
	if (pdirName == "") {
		pdirName = "/";
	}
	string dirName = pathStr.substr(pos + 1);

	int rt = metaDataManager_.delDir(pdirName, dirName);
	if (rt == -1) {
		return -ENOENT;
	} else if (rt == -2) {
		return -ENOTDIR;
	} else if (rt == -3) {
		return -EEXIST;
	} else {
		return 0;
	}
}

/* haven't implemented it */
int MetaServer::rename(const char *from, const char *to)
{
	return 0;
}

int MetaServer::searchDir(const char *path)
{
	int rt = metaDataManager_.searchDir(path);
	if (rt == -1) {
		return -ENOENT;
	} else {
		return 0;
	}
}
