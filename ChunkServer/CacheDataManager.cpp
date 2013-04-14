#include "CacheDataManager.h"

using namespace myfs;

KeyClass::KeyClass(int cacheId, long long chunkId)
 :  cacheId_(cacheId),
	chunkId_(chunkId)
{
}

bool KeyClass::operator<(const KeyClass &r) const
{
	if (chunkId_ == r.chunkId_) {
		return cacheId_ < r.cacheId_;
	} else {
		return chunkId_ < r.chunkId_;
	}
}


CacheDataManager::CacheDataManager()
{
}


CacheDataManager::~CacheDataManager()
{
}

int CacheDataManager::addCacheData(int cacheId, long long chunkId, string data)
{
	KeyClass keyClass(cacheId, chunkId);
	if (cacheDatas_.find(keyClass) != cacheDatas_.end()) {
		cacheDatas_[keyClass] = data;
		return 0;
	}

	cacheDatas_.insert(map<KeyClass, string>::value_type(keyClass, data));
	return 0;
}

int CacheDataManager::delCacheData(int cacheId, long long chunkId)
{
	KeyClass keyClass(cacheId, chunkId);
	if (cacheDatas_.find(keyClass) == cacheDatas_.end()) {
		return -1;
	}

	cacheDatas_.erase(keyClass);
	return 0;
}

string CacheDataManager::searchCacheData(int cacheId, long long chunkId)
{
	KeyClass keyClass(cacheId, chunkId);
	if (cacheDatas_.find(keyClass) == cacheDatas_.end()) {
		return "";
	} else {
		return cacheDatas_[keyClass];
	}
}

int CacheDataManager::getCacheId(long long chunkId)
{
	map<long long, int>::iterator mit = cacheIds_.find(chunkId);
	if (mit == cacheIds_.end()) {
		cacheIds_.insert(map<long long, int>::value_type(chunkId, 0));
	}

	return cacheIds_[chunkId]++;
}

void CacheDataManager::clearCacheId(long long chunkId)
{
	map<long long, int>::iterator mit = cacheIds_.find(chunkId);
	if (mit != cacheIds_.end()) {
		cacheIds_.erase(mit);
	}
}
