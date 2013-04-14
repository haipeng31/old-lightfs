#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <map>
#include <list>
#include <string>
using std::map;
using std::list;
using std::string;

namespace myfs {
class ChunkManager {
public:
	ChunkManager(string, int);

	int addChunk(string);
	int delChunk(string);
	string readChunk(string, int, int);
	int writeChunk(string, int, string); 
	int appendChunk(string, string);

private:
	static const unsigned int kChunkSize = 6 * 1024;
	void createFile(string);
	int rename(string, string);
	map<string, int> chunks_;
	list<string> delChunks_;
	string dir_;
	int used_;
	int size_;
};
}
#endif
