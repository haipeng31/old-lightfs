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
	
	for (int i = 0; i < 50; i++) {
		struct stat stbuf;
		fsClient.getattr("/", &stbuf);
	}
	
	int rt = fsClient.mkdir("/dir1", 0);
	assert(rt == 0);
	struct stat stbuf;
	rt = fsClient.getattr("/dir1", &stbuf);
	assert(rt == 0);

	rt = fsClient.mknod("/file", 0);
	assert(rt == 0);
	rt = fsClient.getattr("/file", &stbuf);
	assert(rt == 0);
	
	rt = fsClient.opendir("/");
	assert(rt == 0);
	rt = fsClient.mkdir("/dir2", 0);
	assert(rt == 0);
#if 0
	rt = fsClient.mkdir("/dir1/dir2", 0);
	assert(rt == 0);
	
	rt = fsClient.opendir("/dir1/dir2");
	assert(rt == 0);
	LOG_INFO << "test success";
#endif
#if 0
	LOG_INFO << "test success";
	
	string rootDir = "/";
	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << rootDir << i;
		assert(fsClient.mknod(ss.str().c_str(), 0) == 0);
	}

	LOG_INFO << "mknod success";

	int rt = fsClient.opendir("/dir1");
	assert(rt != 0);
	//LOG_INFO << "opendir return "<< rt;
	int i;
	cin >> i;
	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << rootDir << "dir" << i;
		assert(fsClient.mkdir(ss.str().c_str(), 0) == 0);
	}

	LOG_INFO << "mkdir success";
	rt = fsClient.opendir("/dir1");
	assert(rt == 0);
	cin >> i;


	Dirent *dirents;
	int cnt = fsClient.readdir(rootDir.c_str(), &dirents);
	assert(cnt == 20);
	LOG_INFO << "readdir success";
	cin >> i;

	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << rootDir << i;
		assert(fsClient.unlink(ss.str().c_str()) == 0);
	}

	LOG_INFO << "unlink success";
	cin >> i;
	for (int i = 0; i < 10; i++) {
		stringstream ss;
		ss << rootDir << "dir" << i;
		assert(fsClient.rmdir(ss.str().c_str()) == 0);
	}

	LOG_INFO << "rmdir success";
	cin >> i;
	LOG_INFO << "past all test";
#endif
	return 0;
}
