#ifndef DIRTREE_H
#define DIRTREE_H

#include "HashTable.h"
#include "INode.h"
#include <list>

using std::list;
namespace myfs {

class DirTree {

public:
	DirTree();

	/* directory operations */
	int addDir(string pdir, string dir);
	int delDir(string pdir, string dir);
	const list<INode *>& readDir(string dir);
	int addFile(string pdir, string file);
	int delFile(string pdir, string file);

	/* file(chunk) operations */
	int addChunk(string path, ChunkInfo &);
	int delChunk(string path);
	ChunkInfo& getChunk(string path, int index);
	ChunkInfo& getLastChunk(string path);
	
	/* used for showing the attributes */
	INode *getINode(string path);
private:
	HashTable dirTable_;	
};
}
#endif
