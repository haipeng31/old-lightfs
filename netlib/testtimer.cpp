#include "EventLoop.h"
#include "base/Logging.h"
#include <time.h>
#include <iostream>

using std::cout;
using namespace mynet;


void atCallback(void *obj)
{
	LOG_TRACE << "In at Call back";
}

void afterCallback(void *obj)
{
	LOG_TRACE << "In after callback";
}

void everyCallback(void *obj)
{
	LOG_TRACE << "In every callback";
}

int main()
{
	EventLoop loop;
	
	struct timespec expire;
	clock_gettime(CLOCK_REALTIME, &expire);
	
	LOG_TRACE << "The time now is " << expire.tv_sec;
	expire.tv_sec += 3;
	struct timespec interval;
	interval.tv_sec = 0;
	interval.tv_nsec = 0;
	loop.runAt(expire, atCallback, NULL);
	interval.tv_sec = 6;
	loop.runAfter(interval, afterCallback, NULL);
	interval.tv_sec = 2;
	loop.runEvery(interval, everyCallback, NULL);
	
	loop.loop();
	return 0;
}
