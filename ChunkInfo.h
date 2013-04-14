#ifndef CHUNKINFO_H
#define CHUNKINFO_H

#include <vector>
#include <string>
using std::vector;
using std::string;
namespace myfs {

class ChunkServerAddr {
public:
	ChunkServerAddr(string name, string ipAddr, int port);
	
	//friend bool operator==(const ChunkServerAddr&, const ChunkServerAddr&);
	string name() const;
	string ipAddr() const ;
	int port() const;
private:
	string name_;
	string ipAddr_;
	int port_;
};
bool operator==(const ChunkServerAddr&, const ChunkServerAddr &);
class ChunkInfo {
public:
	ChunkInfo(string, int);
	
	string chunkName();
	int chunkSize();
	void setChunkSize(int);
	int ncopy();
	void setNcopy(int);
	vector<ChunkServerAddr>& chunkAddrs();
	void setChunkAddr(int, const ChunkServerAddr&);
private:
	string chunkName_;
	int chunkSize_;
	int ncopy_;
	vector<ChunkServerAddr> chunkAddrs_;
};
}
#endif
