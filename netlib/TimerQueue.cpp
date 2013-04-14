#include "TimerQueue.h"
#include "EventLoop.h"
#include "base/Logging.h"
#include <sys/timerfd.h>
#include <string.h>
using namespace mynet;

int TimerQueue::createTimerfd()
{
	int timerfd = timerfd_create(CLOCK_REALTIME, 
				::TFD_NONBLOCK | ::TFD_CLOEXEC);
	if (timerfd < 0) {
		LOG_ERROR << "create timer fd error";
	}

	return timerfd;
}

TimerQueue::TimerQueue(EventLoop *loop)
 :  loop_(loop),
	timerfd_(createTimerfd()),
	channel_(timerfd_)
{
	LOG_TRACE << "timerfd = " << timerfd_;
	channel_.setEvent(Channel::kReadEvent);
	channel_.setArgs((void *)this);
	channel_.setReadCallback(readCallback);
	LOG_TRACE << "fd = " << channel_.fd();
	loop->addChannel(&channel_);
}

TimerQueue::~TimerQueue()
{
	close(timerfd_);
}
void TimerQueue::readCallback(void *objArg)
{
	TimerQueue *timerQueue = (TimerQueue *)objArg;
	timerQueue->handleRead();
}

void TimerQueue::handleRead()
{
	long long temp;
	int n = read(timerfd_, &temp, sizeof(temp));

	if (n != sizeof(temp)) {
		LOG_ERROR << "handleRead read " << n << " bytes other than 8 bytes";
	}
	
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	while (timerQueue_.size()) {
		Timer timer = timerQueue_.min();
		if (timer.compare(now) <= 0) {
			timer.doCallback();
			timerQueue_.extract();

			/* if the timer is repeated, put it to the heap again */
			if (timer.repeated()) {
				timer.updateExpire();
				timerQueue_.insert(timer);
			}
		} else {
			break;
		}
	}

	if (timerQueue_.size()) {
		setTimer(timerQueue_.min());
	}
}

void TimerQueue::setTimer(Timer timer)
{
	struct itimerspec utmr;
	memset(&utmr, 0, sizeof(struct itimerspec));
	utmr.it_value = timer.expire();

	
	int rt = timerfd_settime(timerfd_, ::TFD_TIMER_ABSTIME, &utmr, NULL);
	if (rt < 0) {
		LOG_ERROR << "timerfd_settime error";
	}
}

void TimerQueue::resetTimer()
{
	assert(timerQueue_.size() > 0);
	/* disarm the timer */
	struct itimerspec utmr;
	memset(&utmr, 0, sizeof(struct itimerspec));
	int rt = timerfd_settime(timerfd_, ::TFD_TIMER_ABSTIME, &utmr, NULL);
	if (rt < 0) {
		LOG_ERROR << "timerfd_settime error";
	}

}

void TimerQueue::addTimer(Timer timer)
{
	if (timerQueue_.size() == 0) {
		setTimer(timer);
	} else if (timer < timerQueue_.min()) {
		resetTimer();
		setTimer(timer);
	}
	timerQueue_.insert(timer);
}

