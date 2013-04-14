#ifndef LEASEMANAGER_H
#define LEASEMANAGER_H

#include <time.h>
#include <map>
#include <stdlib.h>
#include <string.h>
using std::map;
namespace myfs {

class Lease {
public:
	Lease();
	Lease(bool setZero);
	Lease(struct timespec serverEndTime);
	bool isZero();
	bool expired();
	void updateEndTime(struct timespec serverEndTime);

private:
	struct timespec subTime(struct timespec dst, struct timespec diff);
	struct timespec endTime_;
	static const struct timespec diffTime;
};

class LeaseManager {
public:
	LeaseManager();
	~LeaseManager();

	int addLease(long long chunkId, struct timespec leaseEndTime);
	int delLease(long long chunkId);

	/* return zero lease if not found */
	Lease searchLease(long long chunkId);
	bool hasLease(long long chunkId);
	int refreshLease(long long chunkId, struct timespec leaseEndTime);

	/* Timer callback func. check wheather the write queue has write request 
	 * if has, try to ask the metaserver to refresh the lease 
	 */
	static void checkWriteCallback(void *objArg, void *args);

	static void delLeaseCallback(void *objArg, void *args);

	int incWriteCount(long long chunkId);
	int decWriteCount(long long chunkId);
	bool hasWrite(long long chunkId);
	
private:
	/* check the write request queue has has write request */
	int checkWrite(long long chunkId);
	
	map<long long, Lease> leases_;
	map<long long, int> writeCounts_;
};
}

#endif
