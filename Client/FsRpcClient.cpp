#include "FsRpcClient.h"
#include "FileSystem.pb.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include "../netlib/base/Logging.h"
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

int FsRpcClient::getattrStub(const char *path, struct stat *stbuf)
{
	/* generate request, and serialize to byte stream */
	GetAttrReq getAttrReq;
	getAttrReq.set_path(path);
	string reqByteStream;
	getAttrReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 1;
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
	assert(repType == 2);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	GetAttrRep getAttrRep;
	getAttrRep.ParseFromString(repMsg);
	
	/* error occur */
	if (getAttrRep.returnval() < 0) {
		free((char *)repByteStream);
		return getAttrRep.returnval();
	}

	GetAttrRep_Stat repStat = getAttrRep.st();
	stbuf->st_mode = repStat.mode();
	stbuf->st_uid = repStat.uid();
	stbuf->st_gid = repStat.gid();
	stbuf->st_size = repStat.size();
	stbuf->st_atime = repStat.atime();
	stbuf->st_mtime = repStat.mtime();
	stbuf->st_ctime = repStat.ctime();
	
	LOG_INFO << "mode = " << std::oct << stbuf->st_mode << std::dec;
	LOG_INFO << "uid = " << stbuf->st_uid;
	LOG_INFO << "gid = " << stbuf->st_gid;
	LOG_INFO << "size = " << stbuf->st_size;
	LOG_INFO << "ctime = " << ctime(&stbuf->st_ctime);
	LOG_INFO << "atime = " << ctime(&stbuf->st_atime);
	LOG_INFO << "mtime = " << ctime(&stbuf->st_mtime);

	free((char *)repByteStream);
	return 0;
}

int FsRpcClient::readdirStub(const char *path, Dirent **dirents)
{
	/* generate request, and serialize to byte stream */
	ReadDirReq readDirReq;
	readDirReq.set_path(path);
	string reqByteStream;
	readDirReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 3;
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
	assert(repType == 4);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	ReadDirRep readDirRep;
	readDirRep.ParseFromString(repMsg);
	
	/* error occur */
	if (readDirRep.returnval() < 0) {
		free((char *)repByteStream);
		return readDirRep.returnval();
	}

	int dirEntSize = readDirRep.dirents_size();
	*dirents = new Dirent[dirEntSize];
	for (int i = 0; i < dirEntSize; i++) {
		ReadDirRep_DirEnt dirent = readDirRep.dirents(i);
		(*dirents)[i].name = dirent.name();
		(*dirents)[i].type = dirent.type();
		LOG_TRACE << "directory entity name " << dirent.name();
	}

	free((char *)repByteStream);
	return dirEntSize;
}

int FsRpcClient::mknodStub(const char *path, mode_t mode)
{
	/* generate request, and serialize to byte stream */
	MkNodReq mkNodReq;
	mkNodReq.set_path(path);
	mkNodReq.set_mode(mode);
	string reqByteStream;
	mkNodReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 5;
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
	assert(repType == 6);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	MkNodRep mkNodRep;
	mkNodRep.ParseFromString(repMsg);
	
	/* error occur */
	if (mkNodRep.returnval() < 0) {
		free((char *)repByteStream);
		return mkNodRep.returnval();
	}

	return 0;
}

int FsRpcClient::mkdirStub(const char *path, mode_t mode)
{
	/* generate request, and serialize to byte stream */
	MkDirReq mkDirReq;
	mkDirReq.set_path(path);
	mkDirReq.set_mode(mode);
	string reqByteStream;
	mkDirReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 7;
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
	assert(repType == 8);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	MkDirRep mkDirRep;
	mkDirRep.ParseFromString(repMsg);
	
	/* error occur */
	if (mkDirRep.returnval() < 0) {
		free((char *)repByteStream);
		return mkDirRep.returnval();
	}

	return 0;
}

int FsRpcClient::unlinkStub(const char *path)
{
	/* generate request, and serialize to byte stream */
	UnlinkReq unlinkReq;
	unlinkReq.set_path(path);
	string reqByteStream;
	unlinkReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 9;
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
	assert(repType == 10);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	UnlinkRep unlinkRep;
	unlinkRep.ParseFromString(repMsg);
	
	/* error occur */
	if (unlinkRep.returnval() < 0) {
		free((char *)repByteStream);
		return unlinkRep.returnval();
	}

	return 0;
}

int FsRpcClient::rmdirStub(const char *path)
{
	/* generate request, and serialize to byte stream */
	RmDirReq rmDirReq;
	rmDirReq.set_path(path);
	string reqByteStream;
	rmDirReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 11;
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
	assert(repType == 12);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	RmDirRep rmDirRep;
	rmDirRep.ParseFromString(repMsg);
	
	/* error occur */
	if (rmDirRep.returnval() < 0) {
		free((char *)repByteStream);
		return rmDirRep.returnval();
	}

	return 0;
}

int FsRpcClient::renameStub(const char *from, const char *to)
{
	/* generate request, and serialize to byte stream */
	RenameReq renameReq;
	renameReq.set_from(from);
	renameReq.set_to(to);
	string reqByteStream;
	renameReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 13;
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
	assert(repType == 14);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	RenameRep renameRep;
	renameRep.ParseFromString(repMsg);
	
	/* error occur */
	if (renameRep.returnval() < 0) {
		free((char *)repByteStream);
		return renameRep.returnval();
	}

	return 0;
}

int FsRpcClient::searchDirStub(const char *path)
{
	/* generate request, and serialize to byte stream */
	SearchDirReq searchDirReq;
	searchDirReq.set_path(path);
	string reqByteStream;
	searchDirReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 15;
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
	assert(repType == 16);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	SearchDirRep searchDirRep;
	searchDirRep.ParseFromString(repMsg);
	
	/* error occur */
	if (searchDirRep.returnval() < 0) {
		free((char *)repByteStream);
		return searchDirRep.returnval();
	}

	return 0;
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

int FsRpcClient::readStub(long long chunkId, int offset, int length, string &data)
{
	/* generate request, and serialize to byte stream */
	ReadReq readReq;
	readReq.set_chunkid(chunkId);
	readReq.set_offset(offset);
	readReq.set_length(length);

	string reqByteStream;
	readReq.SerializeToString(&reqByteStream);

	/* prepend message type field */
	int type = 25;
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
	assert(repType == 26);

	/* parse byte stream to response message */
	string repMsg((char *)repByteStream + 4, repByteStreamLen - 4);
	ReadRep readRep;
	readRep.ParseFromString(repMsg);
	
	data = readRep.data();
	/* error occur */
	if (readRep.returnval() < 0) {
		free((char *)repByteStream);
		return readRep.returnval();
	}

	return data.size();

}
