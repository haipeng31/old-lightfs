#ifndef TIMER_H
#define TIMER_H

#include <time.h>
namespace mynet {
class Timer {
public:
	typedef void (*TimerCallback)(void *objArg);
	Timer(struct timespec, struct timespec, TimerCallback, void *objArg);
	int compare(struct timespec);
	void doCallback();
	bool repeated();
	void updateExpire();
	struct timespec expire();
	static void addTime(struct timespec *, struct timespec);

	bool operator<(const Timer &);
	bool operator>(const Timer &);
private:
	struct timespec expire_;
	struct timespec interval_;
	TimerCallback callback_;
	void *objArg_;
};
}
#endif
