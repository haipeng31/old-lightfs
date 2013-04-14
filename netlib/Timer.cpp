#include "Timer.h"
#include <assert.h>

using namespace mynet;

Timer::Timer(struct timespec expire, struct timespec interval, TimerCallback callback, void *objArg)
 :  expire_(expire),
	interval_(interval),
	callback_(callback),
	objArg_(objArg)
{
}

int Timer::compare(struct timespec time)
{
	if (expire_.tv_sec < time.tv_sec) {
		return -1;
	} else if (expire_.tv_sec > time.tv_sec) {
		return 1;
	} else {
		if (expire_.tv_nsec < time.tv_nsec) {
			return -1;
		} else if (expire_.tv_nsec > time.tv_nsec) {
			return 1;
		} else {
			return 0;
		}
	}
}

void Timer::doCallback()
{
	callback_(objArg_);
}

bool Timer::repeated()
{
	if (interval_.tv_sec || interval_.tv_nsec) {
		return true;
	} else {
		return false;
	}
}

void Timer::updateExpire()
{
	assert(repeated());
	int carry = 0;
	long nsum = expire_.tv_nsec + interval_.tv_nsec;
	carry = nsum / 1000000000;
	if (carry) {
		expire_.tv_nsec = nsum - 1000000000;
		expire_.tv_sec = expire_.tv_sec + interval_.tv_sec + carry;
	} else {
		expire_.tv_nsec = nsum;
		expire_.tv_sec = expire_.tv_sec + interval_.tv_sec;
	}
}

struct timespec Timer::expire()
{
	return expire_;
}

void Timer::addTime(struct timespec *dst, struct timespec src)
{
	int carry = 0;
	long nsum = dst->tv_nsec + src.tv_nsec;
	carry = nsum / 1000000000;
	if (carry) {
		dst->tv_nsec = nsum - 1000000000;
		dst->tv_sec = dst->tv_sec + src.tv_sec + carry;
	} else {
		dst->tv_nsec = nsum;
		dst->tv_sec = dst->tv_sec + src.tv_sec;
	}
}

bool Timer::operator<(const Timer &rs)
{
	if (expire_.tv_sec == rs.expire_.tv_sec) {
		return expire_.tv_nsec < rs.expire_.tv_nsec;
	} else {
		return expire_.tv_sec < rs.expire_.tv_sec;
	}
}

bool Timer::operator>(const Timer &rs)
{
	return !(this->operator<(rs));
}
