#include "CacheDataManager.h"
#include <assert.h>
#include <stdio.h>
using namespace myfs;
int main()
{
	CacheDataManager cacheDataManager;

	cacheDataManager.addCacheData(1,1,"test");
	cacheDataManager.addCacheData(2,1,"testff");

	assert(cacheDataManager.searchCacheData(1,1) == "test");
	assert(cacheDataManager.searchCacheData(2,1) == "testff");
	assert(cacheDataManager.searchCacheData(3,1) == "");

	assert(cacheDataManager.delCacheData(2,1) == 0);
	assert(cacheDataManager.addCacheData(3,1, "testaa") == 0);

	assert(cacheDataManager.searchCacheData(1,1) == "test");
	assert(cacheDataManager.searchCacheData(2,1) == "");
	assert(cacheDataManager.searchCacheData(3,1) == "testaa");

	for (int i = 0; i < 10; i++) {
		assert(cacheDataManager.getCacheId() == i);
	}
	cacheDataManager.clearCacheId();
	for (int i = 0; i < 10; i++) {
		assert(cacheDataManager.getCacheId() == i);
	}
	printf("past all tests\n");
	return 0;

}
