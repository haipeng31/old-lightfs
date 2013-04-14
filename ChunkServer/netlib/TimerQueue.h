#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "base/Heap.h"
#include "Channel.h"
#include "Timer.h"
namespace mynet {
class EventLoop;
class TimerQueue {
public:
	TimerQueue(EventLoop *);
	~TimerQueue();
	void addTimer(Timer);
private:
	static void readCallback(void *);
	void handleRead();
	static int createTimerfd();
	void setTimer(Timer timer);
	void resetTimer();
	EventLoop *loop_;
	int timerfd_;
	Channel channel_;
	Heap<Timer> timerQueue_;
};
}

#endif
