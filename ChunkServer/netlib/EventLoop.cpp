#include "EventLoop.h"
#include "Poller.h"
#include "EPoller.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "base/Logging.h"

using std::cout;
using std::ends;
using std::endl;
using namespace mynet;

EventLoop::EventLoop()
{
	poller_ = new EPoller();
	timerQueue_ = new TimerQueue(this); 
}

EventLoop::~EventLoop()
{
	delete poller_;
	delete timerQueue_;
}

void EventLoop::addChannel(Channel *channel)
{
	poller_->addChannel(channel);
}

void EventLoop::updateChannel(Channel *channel)
{
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
	poller_->removeChannel(channel);
}

void EventLoop::handleEvents()
{
	vector<Channel *>::iterator vit;
	for (vit = activeChannels_.begin(); vit != activeChannels_.end();
				vit++) {
		(*vit)->handleEvent();
	}

	activeChannels_.clear();
}

void EventLoop::printActiveChannels()
{
	for (int i = 0; i < activeChannels_.size(); i++) {
		cout << activeChannels_[i]->fd() << ends << "event" << activeChannels_[i]->getEvent() << endl;
	}
}
void EventLoop::loop()
{

	while (1) {
		activeChannels_.clear();
		poller_->poll(500, activeChannels_);
		
//		LOG_TRACE << "poll end";
//		printActiveChannels();
		handleEvents();
	}

	return;
}

void EventLoop::runAt(struct timespec expire, TimerCallback timerCallback, void *objArg)
{
	struct timespec zeroInterval;
	memset(&zeroInterval, 0, sizeof(struct timespec));
	Timer timer(expire, zeroInterval, timerCallback, objArg);
	timerQueue_->addTimer(timer);
}

void EventLoop::runAfter(struct timespec interval, TimerCallback timerCallback, void *objArg)
{
	struct timespec expire;
	memset(&expire, 0, sizeof(struct timespec));
	clock_gettime(CLOCK_REALTIME, &expire);
	Timer::addTime(&expire, interval);
		
	struct timespec zeroInterval;
	memset(&zeroInterval, 0, sizeof(struct timespec));
	Timer timer(expire, zeroInterval, timerCallback, objArg);
	timerQueue_->addTimer(timer);
}

void EventLoop::runEvery(struct timespec interval, TimerCallback timerCallback, void *objArg)
{
	struct timespec expire;
	memset(&expire, 0, sizeof(struct timespec));
	clock_gettime(CLOCK_REALTIME, &expire);
	Timer::addTime(&expire, interval);
		
	Timer timer(expire, interval, timerCallback, objArg);
	timerQueue_->addTimer(timer);
}


