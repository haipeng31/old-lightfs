#include "LeaseManager.h"

using namespace myfs;


Lease::Lease()
{
	clock_gettime(CLOCK_REALTIME, &endTime_);
}

Lease::Lease(bool setZero)
{
	if (setZero) {
		memset(&endTime_, 0, sizeof(struct timespec));
	} else {
		Lease();
	}
}

Lease::Lease(struct timespec serverTime)
{
	endTime_ = subTime(serverTime, diffTime);
}

bool Lease::isZero()
{
	if ((endTime_.tv_sec + endTime_.tv_nsec) == 0) {
		return true;
	} else {
		return false;
	}
}

bool Lease::expired()
{
	struct timespec now;

	clock_gettime(CLOCK_REALTIME, &now);
	if (now.tv_sec == endTime_.tv_sec) {
		return now.tv_nsec > endTime_.tv_nsec;
	} else {
		return now.tv_sec > endTime_.tv_sec;
	}
}

void Lease::updateEndTime(struct timespec serverTime)
{
	endTime_ = subTime(serverTime, diffTime);
}

const struct timespec Lease::diffTime = {1, 0};

struct timespec Lease::subTime(struct timespec dst, struct timespec diff)
{
	struct timespec temp;
	memset(&temp, 0, sizeof(struct timespec));

	if (dst.tv_nsec < diff.tv_nsec) {
		temp.tv_nsec = dst.tv_nsec - diff.tv_nsec + 1000000000;
		temp.tv_sec = dst.tv_sec - diff.tv_sec - 1;
	} else {
		temp.tv_nsec = dst.tv_nsec - diff.tv_nsec;
		temp.tv_sec = dst.tv_sec - diff.tv_sec;
	}
	
	return temp;
}

LeaseManager::LeaseManager()
{
}


LeaseManager::~LeaseManager()
{
}

int LeaseManager::addLease(long long chunkId, struct timespec leaseEndTime)
{
	if (leases_.find(chunkId) != leases_.end()) {
		return -1;
	}

	Lease newLease(leaseEndTime);
	leases_.insert(map<long long, Lease>::value_type(chunkId, newLease));

	/* add checkwrite callback to eventloop 
	 * fix me 
	 */

	return 0;
}

int LeaseManager::delLease(long long chunkId)
{
	if (leases_.find(chunkId) == leases_.end()) {
		return -1;
	}

	leases_.erase(chunkId);
	return 0;
}

Lease LeaseManager::searchLease(long long chunkId)
{
	if (leases_.find(chunkId) == leases_.end()) {
		return Lease(true);
	}

	return leases_[chunkId];
}

bool LeaseManager::hasLease(long long chunkId)
{
	Lease lease = searchLease(chunkId);
	if (lease.isZero()) {
		return false;
	} else {
		return !lease.expired();
	}
}

int LeaseManager::refreshLease(long long chunkId, struct timespec leaseEndTime)
{
	if (leases_.find(chunkId) == leases_.end()) {
		return -1;
	}
	leases_[chunkId].updateEndTime(leaseEndTime);
	return 0;
}

void LeaseManager::checkWriteCallback(void *objArg, void *args)
{
	LeaseManager *leaseManager = (LeaseManager *)objArg;
	long long chunkId = *(long long *)args;

	int rt = leaseManager->checkWrite(chunkId);
	if (rt == -1) {
		return;
	} else if (rt == 0) {
		/* ask the meta server to longer lease 
		 * fix me */
		struct timespec newtime;
		leaseManager->refreshLease(chunkId, newtime);
	} else {
		/* delete the lease when expired, add callback to eventloop
		 * fix me */

	}
}

void LeaseManager::delLeaseCallback(void *objArg, void *args)
{
	LeaseManager *leaseManager = (LeaseManager *)objArg;
	long long chunkId = *(long long *)args;

	leaseManager->delLease(chunkId);
}

int LeaseManager::incWriteCount(long long chunkId)
{
	if (writeCounts_.find(chunkId) == writeCounts_.end()) {
		writeCounts_.insert(map<long long, int>::value_type(chunkId, 1));
	} else {
		writeCounts_[chunkId]++;
	}

	return 0;
}

int LeaseManager::decWriteCount(long long chunkId)
{
	if (writeCounts_.find(chunkId) == writeCounts_.end()) {
		return -1;
	} else {
		writeCounts_[chunkId]--;
		if (writeCounts_[chunkId] == 0) {
			writeCounts_.erase(chunkId);
		}
		return 0;
	}
}

bool LeaseManager::hasWrite(long long chunkId)
{
	return (writeCounts_.find(chunkId) != writeCounts_.end()) && (writeCounts_[chunkId] > 0);
}

int LeaseManager::checkWrite(long long chunkId)
{
	if (leases_.find(chunkId) == leases_.end()) {
		return -1;
	}
	
	if (hasWrite(chunkId)) {
		return 0;
	} else {
		return 1;
	}
}


