#include "MetaDataManager.h"
#include "netlib/base/Logging.h"
#include "ChunkInfo.h"
#include "IDGenerator.h"
#include <sstream>
#include <vector>
#include <assert.h>

using namespace myfs;
using mynet::Logger;
using std::vector;
using std::stringstream;

MetaDataManager::MetaDataManager()
{
}

int MetaDataManager::addDir(string pdir, string dir)
{
	return dirTree_.addDir(pdir, dir);
}

int MetaDataManager::delDir(string pdir, string dir)
{
	return dirTree_.delDir(pdir, dir);
}

const list<INode *>& MetaDataManager::readDir(string dir)
{
	return dirTree_.readDir(dir);
}

int MetaDataManager::searchDir(string dir)
{
	INode *inode = dirTree_.getINode(dir);

	if ((inode != NULL) && (inode->type() == INode::Dir)) {
		return 0;
	} else {
		return -1;
	}
}

int MetaDataManager::addFile(string pdir, string file)
{
	return dirTree_.addFile(pdir, file);
}

int MetaDataManager::delFile(string pdir, string file)
{
	return dirTree_.delFile(pdir, file);
}
	
int MetaDataManager::addChunk(string path, int ncopy)
{
	assert(chunkServers_.size() >= ncopy);

	vector<ChunkServerState> selectedServers;
	//selectedServers.resize(ncopy);

	/* select ncopy chunk servers to put the new chunks */
	for (int i = 0; i < ncopy; i++) {
		selectedServers.push_back(chunkServers_.top());
		chunkServers_.pop();
	}

	/* generate the universal chunk id */
	long long chunkId = IDGenerator::genId();
	stringstream ss;
	ss << chunkId;

	/* request the chunk servers to add chunk
	 * doesn't implement now, fix me
	 */

	/* update meta data */
	ChunkInfo info(ss.str(), 3);
	for (int i = 0; i < selectedServers.size(); i++) {
		info.setChunkAddr(i, selectedServers[i].serverAddr());
	}
	dirTree_.addChunk(path, info);

	for (int i = 0; i < selectedServers.size(); i++) {
		selectedServers[i].addChunk(ss.str());
		chunkServers_.push(selectedServers[i]);
	}

}

int MetaDataManager::delChunk(string path)
{
	ChunkInfo info = dirTree_.getLastChunk(path);

	/* ask the chunk servers to delete the chunk 
	 * doesn't implement now 
	 */
	
	/* rather silly and low performance, priority queue
	 * doesn't support change item's value
	 * */
	vector<ChunkServerState> svec;
	while (chunkServers_.size() > 0) {
		svec.push_back(chunkServers_.top());
		chunkServers_.pop();

		vector<ChunkServerAddr> serverAddrs = info.chunkAddrs();
		for (int i = 0; i < serverAddrs.size(); i++) {
			if (svec[svec.size()-1].serverAddr() == serverAddrs[i]) {
				svec[svec.size()-1].delChunk(info.chunkName());
			}
		}
	}
	for (int i = 0; i < svec.size(); i++) {
		chunkServers_.push(svec[i]);
	}

	dirTree_.delChunk(path);

	return 0;
}

ChunkInfo& MetaDataManager::getChunk(string path, int index)
{
	return dirTree_.getChunk(path, index);
}

INode *MetaDataManager::getINode(string path)
{
	return dirTree_.getINode(path);
}

int MetaDataManager::addChunkServer(ChunkServerState &serverState)
{
	chunkServers_.push(serverState);
	return 0;
}

/* silly implemention now */
int MetaDataManager::delChunkServer(ChunkServerAddr &serverAddr)
{
	vector<ChunkServerState> svec;
	while (chunkServers_.size() > 0) {
		svec.push_back(chunkServers_.top());
		chunkServers_.pop();
		if (svec[svec.size()-1].serverAddr() == serverAddr) {
			svec.pop_back();
			break;
		}
	}

	for (int i = 0; i < svec.size(); i++) {
		chunkServers_.push(svec[i]);
	}

	return 0;
}
