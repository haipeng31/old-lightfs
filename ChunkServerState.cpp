#include "ChunkServerState.h"
#include <iostream>
#include <assert.h>


using std::cout;
using std::endl;
using namespace myfs;

ChunkServerState::ChunkServerState(ChunkServerAddr &serverAddr, int used, int size)
 :  serverAddr_(serverAddr),
	used_(used),
	size_(size)
{
}

ChunkServerState::ChunkServerState(ChunkServerAddr &serverAddr,
			list<string> &chunks, int used, int size)
 :  serverAddr_(serverAddr),
	chunks_(chunks),
	used_(used),
	size_(size)
{
}

const ChunkServerAddr &ChunkServerState::serverAddr()
{
	return serverAddr_;
}

const list<string> &ChunkServerState::chunkList()
{
	return chunks_;
}

double ChunkServerState::load() const
{
	assert(size_ > 0);
	return double(used_) / double(size_);
}

int ChunkServerState::used()
{
	return used_;
}

int ChunkServerState::size()
{
	return size_;
}

void ChunkServerState::addChunk(string chunkId)
{
	chunks_.push_back(chunkId);
}

void ChunkServerState::delChunk(string chunkId)
{
	chunks_.remove(chunkId);
}

void ChunkServerState::setUsed(int used)
{
	used_ = used;
}

void ChunkServerState::setSize(int size)
{
	size_ = size;
}


bool myfs::operator<(const ChunkServerState &ls, const ChunkServerState &rs)
{
	return (ls.load() < rs.load());
}

/* test code for class ChunkServerState 
int main()
{
	ChunkServerAddr serverAddr("test", "127.0.0.1", 5555);
	ChunkServerState serverState(serverAddr, 0, 1000);

	serverState.addChunk("aa");
	serverState.addChunk("bb");

	list<string> testlist = serverState.chunkList();
	for (list<string>::iterator it = testlist.begin();
				it != testlist.end(); it++) {
		cout << *it << std::endl;
	}
	
	serverState.setUsed(100);
	cout << serverState.load() << std::endl;
	return 0;
}
*/




