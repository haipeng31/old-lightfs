#pragma once

#ifndef CACHEDATAMANAGER_H
#define CACHEDATAMANAGER_H

#include <map>
#include <string>
using std::map;
using std::string;


namespace myfs {


class KeyClass {
public:
	KeyClass(int cacheId, long long chunkId);
	bool operator<(const KeyClass &) const;
private:
	long long chunkId_;
	int cacheId_;
};

class CacheDataManager {
public:
	CacheDataManager();
	~CacheDataManager();

	/* if exist, replace the old with the new one */
	int addCacheData(int cacheId, long long chunkId, string data);
	/* if not exist, return -1, else, return 0 */
	int delCacheData(int cacheId, long long chunkId);
	/* if not exist, return "" */
	string searchCacheData(int cacheId, long long chunkId);

	int getCacheId(long long chunkId);
	void clearCacheId(long long chunkId);
private:
	map<KeyClass, string> cacheDatas_;
	map<long long, int> cacheIds_;
};
}

#endif
