#include "INode.h"
#include <assert.h>
#include <sys/stat.h>
using namespace myfs;

INode::INode(string key, filetype type)
 :  key_(key),
	type_(type),
	size_(0),
	atime_(time(NULL)),
	mtime_(time(NULL)),
	ctime_(time(NULL)),
	auth_(0777),
	uid_(0),
	gid_(0)
{
}

const string& INode::key()
{
	return key_;
}

int INode::type()
{
	return type_;
}

const list<INode *>& INode::children()
{
	return children_;
}

void INode::addChild(INode *child)
{
	children_.push_back(child);
}

void INode::delChild(INode *child)
{
	children_.remove(child);
}

INode *INode::searchChild(string key)
{
	list<INode *>::iterator lit;
	for (lit = children_.begin(); lit != children_.end(); lit++) {
		if ((*lit)->key() == key) {
			return (*lit);
		}
	}

	return NULL;
}

int INode::childCnt()
{
	assert(type_ == Dir);
	return children_.size();
}

int INode::size()
{
	return size_;
}

vector<ChunkInfo>& INode::chunks()
{
	return chunks_;
}

void INode::addChunk(const ChunkInfo &chunkInfo)
{
	chunks_.push_back(chunkInfo);
}

void INode::delChunk()
{
	assert(chunks_.size() >= 0);
	chunks_.pop_back();
}

ChunkInfo &INode::getChunk(int index)
{
	assert((index >= 0) && (index < chunks_.size()));
	return chunks_[index];
}

int INode::chunkCnt()
{
	return chunks_.size();
}
time_t INode::atime()
{
	return atime_;
}

time_t INode::mtime()
{
	return mtime_;
}

time_t INode::ctime()
{
	return ctime_;
}

void INode::updateAtime()
{
	atime_ = time(NULL);
}

void INode::updateMtime()
{
	mtime_ = time(NULL);
}

void INode::updateCtime()
{
	ctime_ = time(NULL);
}

int INode::auth()
{
	return auth_;
}

void INode::setAuth(int auth)
{
	auth_ = auth;
}

int INode::uid()
{
	return uid_;
}

int INode::setUid(int uid)
{
	uid_ = uid;
}

int INode::gid()
{
	return gid_;
}

int INode::setGid(int gid)
{
	gid_ = gid;
}

int INode::mode()
{
	int type;
	if (type_ == Dir) {
		type = S_IFDIR;
	} else {
		type = S_IFREG;
	}

	return type + auth_;
}

int INode::fileType()
{
	if (type_ == Dir) {
		return S_IFDIR;
	} else {
		return S_IFREG;
	}
}
