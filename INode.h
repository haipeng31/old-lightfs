#ifndef INODE_H
#define INODE_H

#include <time.h>
#include <string>
#include <list>
#include "ChunkInfo.h"
using std::string;
using std::list;
namespace myfs {

class INode {
public:
	enum filetype {
		Dir,
		File
	};
	INode(string key, filetype type);
	
	const string& key();
	int type();
	const list<INode *>& children();
	void addChild(INode *);
	void delChild(INode *);
	INode *searchChild(string key);
	int childCnt();
	int size();
	
	vector<ChunkInfo>& chunks();
	void addChunk(const ChunkInfo &);
	ChunkInfo &getChunk(int index);
	int chunkCnt();

	/* only delete the last chunk now, maybe need to change */
	void delChunk();

	time_t atime();
	time_t mtime();
	time_t ctime();
	void updateAtime();
	void updateMtime();
	void updateCtime();

	int auth();
	void setAuth(int);
	int uid();
	int setUid(int);
	int gid();
	int setGid(int);
	int mode();
	int fileType();
private:
	
	/* key_ of dir is full path, key_ of file is only a name */
	string key_;
	list<INode *> children_;
	
	int size_;
	
	/* if dir, size of chunks_ is 0 */
	vector<ChunkInfo> chunks_;

	/* 0 is dir, 1 is file */
	filetype type_;

	/* time attributes */
	time_t atime_;
	time_t mtime_;
	time_t ctime_;

	/* authority attribute, not used now */
	int auth_;

	/* uid, gid */
	int uid_;
	int gid_;
};
}
#endif
