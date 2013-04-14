#include "ChunkServer.h"
#include "FileSystem.pb.h"
#include "netlib/base/Logging.h"
#include "FsClient.h"
#include <iostream>
#include <sstream>

using mynet::Logger;
using std::stringstream;
using namespace myfs;

ChunkServer::ChunkServer(const char *name, const char *ipAddr, int port, int nthread, int nwriteThread, string dir, int maxchunk)
 :	loop_(),
	rpcServer_(&loop_, name, ipAddr, port, nthread, nwriteThread),
	chunkManager_(dir, maxchunk),
	leaseManager_(),
	cacheDataManager_(),
	cSConManager_()
{
	/* register rpcs */
	rpcServer_.registerRpc(17, addChunkCallback, (void *)this);
	rpcServer_.registerRpc(19, delChunkCallback, (void *)this);
	rpcServer_.registerRpc(21, writeDataCallback, (void *)this);
	rpcServer_.registerRpc(23, writeCallback, (void *)this);
	rpcServer_.registerRpc(25, readCallback, (void *)this);
}

ChunkServer::~ChunkServer()
{
}

void ChunkServer::start()
{
	rpcServer_.start();
	loop_.loop();
}

void *ChunkServer::addChunkCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	ChunkServer *chunkServer = (ChunkServer *)objArg;
	return chunkServer->addChunkCallback(reqMsg, reqMsgLen, repMsgLen);
}

void *ChunkServer::delChunkCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	ChunkServer *chunkServer = (ChunkServer *)objArg;
	return chunkServer->delChunkCallback(reqMsg, reqMsgLen, repMsgLen);
}

void *ChunkServer::writeDataCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	ChunkServer *chunkServer = (ChunkServer *)objArg;
	return chunkServer->writeDataCallback(reqMsg, reqMsgLen, repMsgLen);
}

void *ChunkServer::writeCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	ChunkServer *chunkServer = (ChunkServer *)objArg;
	return chunkServer->writeCallback(reqMsg, reqMsgLen, repMsgLen);
}

void *ChunkServer::readCallback(void *objArg, const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	ChunkServer *chunkServer = (ChunkServer *)objArg;
	return chunkServer->readCallback(reqMsg, reqMsgLen, repMsgLen);
}

void *ChunkServer::addChunkCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	AddChunkReq addChunkReq;
	addChunkReq.ParseFromString(reqByteStream);
	
	long long chunkId = addChunkReq.chunkid();
	stringstream ss;
	ss << chunkId;
	int rt = chunkManager_.addChunk(ss.str());

	AddChunkRep addChunkRep;
	addChunkRep.set_returnval(rt);

	string repByteStream;
	addChunkRep.SerializeToString(&repByteStream);
	int type = 18;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
}

void *ChunkServer::delChunkCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	DelChunkReq delChunkReq;
	delChunkReq.ParseFromString(reqByteStream);
	
	long long chunkId = delChunkReq.chunkid();
	stringstream ss;
	ss << chunkId;
	int rt = chunkManager_.delChunk(ss.str());

	DelChunkRep delChunkRep;
	delChunkRep.set_returnval(rt);

	string repByteStream;
	delChunkRep.SerializeToString(&repByteStream);
	int type = 20;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
}

/* pipeline to send data first, data will be save in cache first */
void *ChunkServer::writeDataCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	WriteDataReq writeDataReq;
	writeDataReq.ParseFromString(reqByteStream);
	
	int size = writeDataReq.serveraddrs_size();
	vector<ChunkServerAddr> csAddrs;
	for (int i = 1; i < size; i++) {
		WriteDataReq_CSAddr csAddr = writeDataReq.serveraddrs(i);
		ChunkServerAddr serverAddr(csAddr.ipaddr(), csAddr.port());
		csAddrs.push_back(serverAddr);
	}
	int remoteRt = 0, rt = 0;
	if (size >= 1) {
		FsClient *fsClient = cSConManager_.getOrCreateCSCon(writeDataReq.serveraddrs(0).ipaddr(), writeDataReq.serveraddrs(0).port());
		fsClient->checkState();		
		/*
		CSAddr *csAddr = repostReq.add_serveraddrs();
		csAddr->set_ipaddr(csAddrs[1].ipAddr());
		csAddr->set_port(csAddrs[1].port());

		repostReq.set_chunkid(writeDataReq.chunkid());
		repostReq.set_cacheid(writeDataReq.cacheid());
		repostReq.set_data(writeDataReq.data());
		*/
		fsClient->checkState();
		remoteRt = fsClient->writeData(csAddrs, writeDataReq.chunkid(), writeDataReq.cacheid(), writeDataReq.data());

	}
	rt = cacheDataManager_.addCacheData(writeDataReq.cacheid(), writeDataReq.chunkid(), writeDataReq.data());
	if (rt != 0 || remoteRt != 0) {
		rt = -1;
	}
	WriteDataRep writeDataRep;
	writeDataRep.set_returnval(rt);

	string repByteStream;
	writeDataRep.SerializeToString(&repByteStream);
	int type = 22;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
}

/* write data from cache to local file system */
void *ChunkServer::writeCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	WriteReq writeReq;
	writeReq.ParseFromString(reqByteStream);
	
	int remoteRt = 0, rt = -1;
	
	int size = writeReq.serveraddrs_size();
	int cacheId = writeReq.cacheid();
	long long chunkId = writeReq.chunkid();
	int offset = writeReq.offset();

	/* primary chunkserver send write request to secondary chunkservers */
	for (int i = 0; i < size; i++) {
		FsClient *fsClient = cSConManager_.getOrCreateCSCon(writeReq.serveraddrs(i).ipaddr(), writeReq.serveraddrs(i).port());
		
		vector<ChunkServerAddr> csAddrs;
		csAddrs.clear();
		remoteRt = fsClient->write(csAddrs, chunkId, cacheId, offset);
	}
	
	/* write data from cache to local disk */
	string data = cacheDataManager_.searchCacheData(cacheId, chunkId);
	if (data != "") {
		cacheDataManager_.delCacheData(cacheId, chunkId);
		stringstream ss;
		ss << chunkId;
		
		rt = chunkManager_.writeChunk(ss.str(), offset, data);
	}
	
	if (rt != 0 || remoteRt != 0) {
		rt = -1;
	}
	WriteRep writeRep;
	writeRep.set_returnval(rt);

	string repByteStream;
	writeRep.SerializeToString(&repByteStream);
	int type = 24;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
}

void *ChunkServer::readCallback(const void *reqMsg, int reqMsgLen, int *repMsgLen)
{
	string reqByteStream((char *)reqMsg, reqMsgLen);
	ReadReq readReq;
	readReq.ParseFromString(reqByteStream);
	
	long long chunkId = readReq.chunkid();
	int offset = readReq.offset();
	int length = readReq.length();

	stringstream ss;
	ss << chunkId;
	string data = chunkManager_.readChunk(ss.str(), offset, length);

	ReadRep readRep;
	readRep.set_data(data);
	readRep.set_returnval(data.size());

	string repByteStream;
	readRep.SerializeToString(&repByteStream);
	int type = 26;
	repByteStream.insert(0, (char *)&type, 4);
	*repMsgLen = repByteStream.size();

	char *repMsg = (char *)malloc(*repMsgLen);
	memcpy(repMsg, repByteStream.c_str(), *repMsgLen);
	return repMsg;
}
