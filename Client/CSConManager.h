#ifndef CSCONMANAGER_H
#define CSCONMANAGER_H

#include <map>
#include <string>
using std::string;
using std::map;

namespace myfs {

class FsClient;

class ChunkServerAddr {
public:
	ChunkServerAddr(string, int);
	string ipAddr();
	int port();
	bool operator<(const ChunkServerAddr &) const;
private:
	string ipAddr_;
	int port_;
};

class CSConManager {
public:
	FsClient *getOrCreateCSCon(string ipAddr, int port);
	FsClient *getOrCreateCSCon(ChunkServerAddr chunkServerAddr);
private:
	map<ChunkServerAddr, FsClient *> CSConns_;
};

}

#endif
