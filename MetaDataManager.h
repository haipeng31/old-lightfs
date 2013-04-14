#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include "DirTree.h"
#include "ChunkServerState.h"
#include "INode.h"

#include <string>
#include <list>
#include <queue>  /* may be I should implement a priority queue myself */
using std::priority_queue;
using std::list;
using std::string;
namespace myfs {
class MetaDataManager {
public:
	MetaDataManager();

	/* directory operations */
	int addDir(string pdir, string dir);
	int delDir(string pdir, string dir);
	const list<INode *>& readDir(string dir);
	int searchDir(string dir);
	int addFile(string pdir, string file);
	int delFile(string pdir, string file);

	/* chunk operations */
	int addChunk(string path, int ncopy);
	int delChunk(string path);
	ChunkInfo& getChunk(string path, int index);

	/* used for showing the attributes */
	INode *getINode(string path);

	/* chunkserver operations */
	int addChunkServer(ChunkServerState &serverState);
	int delChunkServer(ChunkServerAddr &serverAddr);
private:
	DirTree dirTree_;
	priority_queue<ChunkServerState> chunkServers_;
};

}
#endif
