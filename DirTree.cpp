#include "DirTree.h"
#include "netlib/base/Logging.h"
#include <sstream>
#include <assert.h>

using mynet::Logger;
using namespace myfs;
using std::stringstream;

DirTree::DirTree()
{
	INode *rootNode = new INode(string("/"), INode::Dir);
	dirTable_.insert(string("/"), rootNode);
}

/*
 * 0 means success, -1 means dir exist
 * -2 means pdir doesn't exist
 */
int DirTree::addDir(string pdir, string dir)
{
	INode *pDirNode = dirTable_.search(pdir);
	if (pDirNode == NULL) {
		LOG_TRACE << "Parent dir doesn't exist";
		return -2;
	}

	assert(pDirNode != NULL);

	string key = "";
	if (pdir == "/") {
		key = pdir + dir;
	} else {
		key = pdir + "/" + dir;
	}
	INode *newDirNode = dirTable_.search(key);
	if (newDirNode != NULL) {
		LOG_TRACE << "New dir already exist";
		return -1;
	}
	newDirNode = new INode(key, INode::Dir);

	/* add new hash table item */
	dirTable_.insert(key, newDirNode);
	assert(dirTable_.search(key) != NULL);
	/* add to the pdir's children list */
	pDirNode->addChild(newDirNode);

	return 0;
}	

/*
 * -2 means parent dir doesn't exist,
 * -1 means dir doesn't exits,
 * -3 means dir is not empty
 */
int DirTree::delDir(string pdir, string dir)
{
	INode *pDirNode = dirTable_.search(pdir);
	if (pDirNode == NULL) {
		LOG_TRACE << "Parent dir doesn't exist";
		return -2;
	}

	assert(pDirNode != NULL);
	
	string key = "";
	if (pdir == "/") {
		key = pdir + dir;
	} else {
		key = pdir + "/" + dir;
	}

	INode *childDirNode = dirTable_.search(key);
	if (childDirNode == NULL) {
		LOG_TRACE << "child dir doesn't exist";
		return -1;
	}
	if (childDirNode->childCnt() != 0) {
		LOG_TRACE << "The dir is not empty";
		return -3;
	}
	/* delete from the hash table */
	dirTable_.erase(key);

	/* delete from the pdir's children list */
	pDirNode->delChild(childDirNode);
	
	delete childDirNode;
	return 0;
}
	
const list<INode *>& DirTree::readDir(string dir)
{
	INode *inode = dirTable_.search(dir);
	if (inode == NULL) {
		LOG_TRACE << "Dir " << dir << " not found";
	}
	return inode->children();
}

int DirTree::addFile(string pdir, string file)
{
	INode *pDirNode = dirTable_.search(pdir);
	if (pDirNode == NULL) {
		LOG_TRACE << "Parent dir doesn't exist";
		return -2;
	}
	
	if (pDirNode->searchChild(file) != NULL) {
		LOG_TRACE << "file already exist";
		return -1;
	}
	INode *newnode = new INode(file, INode::File);
	pDirNode->addChild(newnode);
	
	return 0;
}

/* 
 * we should ask the chunk servers to
 * delete the chunks when deleting file
 * fix me
 */
int DirTree::delFile(string pdir, string file)
{
	INode *pDirNode = dirTable_.search(pdir);
	if (pDirNode == NULL) {
		LOG_TRACE << "Parent dir doesn't exist";
		return -2;
	}
	
	INode *node;
	if ((node = pDirNode->searchChild(file)) == NULL) {
		LOG_TRACE << "file doesn't exist";
		return -1;
	}
	
	pDirNode->delChild(node);
	delete node;
	
	return 0;
}

	
int DirTree::addChunk(string path, ChunkInfo &info)
{
	INode *node = getINode(path);
	assert((node != NULL) && (node->type() == INode::File));
	
	/* generate the universal chunk id 
	long long chunkId = IDGenerator::genId();
	stringstream ss;
	ss << chunkId;
	//string chunkName = chunkId;

	ChunkInfo info(ss.str(), 3);

	* using the chunk server information to choose the chunk placements 
	 * I set three test chunkserver here, fix me
	 *
	for (int i = 0; i < 3; i++) {
		ChunkServerAddr addr("TestServer", "127.0.0.1", 5555);
		info.setChunkAddr(i, addr);
	}
	*/
	node->addChunk(info);
	
	return 0;
}

int DirTree::delChunk(string path)
{
	INode *node = getINode(path);
	assert((node != NULL) && (node->type() == INode::File));
	
	/* ask the chunkserver to delete the chunks */


	/* delete the chunk meta data */
	node->delChunk();

	return 0;
}

ChunkInfo& DirTree::getChunk(string path, int index)
{
	INode *node = getINode(path);
	assert((node != NULL) && (node->type() == INode::File));
	assert((index >= 0) && (index < node->chunkCnt()));
	
	return node->getChunk(index);
}

ChunkInfo& DirTree::getLastChunk(string path)
{
	INode *node = getINode(path);
	assert((node != NULL) && (node->type() == INode::File));
	
	return node->getChunk(node->chunkCnt()-1);
}
INode *DirTree::getINode(string path)
{
	INode *dirnode = dirTable_.search(path);
	if (dirnode != NULL) {
		return dirnode;
	}
	int pos = path.find_last_of('/');
	string dir = path.substr(0, pos);
	if (dir == "") {
		dir = "/";
	}
	string file = path.substr(pos+1);

	INode *pdirnode = dirTable_.search(dir);
	if (pdirnode == NULL) {
		LOG_TRACE << "Dir doesn't exist";
		return NULL;
	}

	return pdirnode->searchChild(file);
}
