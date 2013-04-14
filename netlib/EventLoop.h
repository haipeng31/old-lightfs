#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "Channel.h"
#include <vector>
#include "Timer.h"
#include "TimerQueue.h"

namespace mynet {

class Poller;
using std::vector;

class EventLoop {
 public:
	 typedef void (*TimerCallback)(void *objArg);
	 EventLoop();
	 ~EventLoop();
	 void addChannel(Channel *);
	 void updateChannel(Channel *);
	 void removeChannel(Channel *);
	 void loop();
	 void runAt(struct timespec expire, TimerCallback timerCallback, void *objArg);
	 void runAfter(struct timespec interval, TimerCallback timerCallback, void *objArg);
	 void runEvery(struct timespec interval, TimerCallback timerCallback, void *objArg);

 private:
	 void handleEvents();
	 void printActiveChannels();
	 Poller *poller_;
	 typedef vector<Channel *> ChannelList;
	 ChannelList activeChannels_;
	 TimerQueue *timerQueue_;
};

}
#endif
