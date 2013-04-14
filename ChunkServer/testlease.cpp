#include "LeaseManager.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
using namespace myfs;
int main()
{
	LeaseManager leaseManager;
	struct timespec endTime;
	clock_gettime(CLOCK_REALTIME, &endTime);
	
	/* add first lease */
	endTime.tv_sec += 10;
	leaseManager.addLease(1, endTime);
	
	/* add second lease */
	endTime.tv_sec += 5;
	leaseManager.addLease(2, endTime);

	/* check the leases */
	assert(leaseManager.hasLease(1));
	assert(leaseManager.hasLease(2));

	printf("After 11 seconds\n");
	
	sleep(11);
	assert(!leaseManager.hasLease(1));
	assert(leaseManager.hasLease(2));

	sleep(7);
	assert(!leaseManager.hasLease(2));

	endTime.tv_sec += 10;
	leaseManager.refreshLease(1, endTime);
	assert(leaseManager.hasLease(1));

	printf("past all test\n");
}
