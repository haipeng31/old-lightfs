#include "FsClient.h"
#include <sys/stat.h>
#include "../netlib/base/Logging.h"
#include <sstream>
#include <assert.h>
#include <iostream>

using std::stringstream;
using std::cin;
using namespace myfs;
using mynet::Logger;
int main()
{
	FsClient fsClient("test", "127.0.0.1", 5555);
	FsClient fsClient1("test1", "127.0.0.1", 5556);
	FsClient fsClient2("test2", "127.0.0.1", 5557);
	for (int i = 0; i < 10; i++) {
		int rt = fsClient.addChunk(i);
		assert(rt == 0);
	}
	for (int i = 0; i < 10; i++) {
		int rt = fsClient.delChunk(i);
		assert(rt == 0);
	}
	
	int rt;
	rt = fsClient.addChunk(11);
	assert(rt == 0);

	rt = fsClient1.addChunk(11);
	assert(rt == 0);
	
    rt = fsClient2.addChunk(11);
	assert(rt == 0);
	

	vector<ChunkServerAddr> serverAddrs;
	for(int i = 0; i < 2; i++) {
		ChunkServerAddr serverAddr("127.0.0.1", 5556 + i);
		serverAddrs.push_back(serverAddr);
	}

	rt = fsClient.writeData(serverAddrs, 11, 1, "hello world");
	assert(rt == 0);
	LOG_INFO << "finishing writing data";
	rt = fsClient.write(serverAddrs, 11, 1, 0);
	assert(rt == 0);
	string data;
	int len = fsClient.read(11, 0, 5, data);
	LOG_INFO << data;
	LOG_INFO << len;
	assert(len == 5);
	LOG_INFO << "past all test";
	return 0;
}
