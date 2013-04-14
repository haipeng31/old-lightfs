#ifndef EPOLLER_H
#define EPOLLER_H

#include "Poller.h"
#include <sys/epoll.h>
#include <vector>
#include <map>

using std::vector;
using std::map;

namespace mynet {

class Channel;

class EPoller : public Poller{
public:
	EPoller();
	~EPoller();
	void addChannel(Channel *);
	void updateChannel(Channel *);
	void removeChannel(Channel *);
	void poll(int timeout, ChannelList &);
private:
	int	epollToEvent(int epollEvent);
	void update(int op, Channel *channel);
	void fillActiveChannels(int nEvents, ChannelList &);
	static const int MaxEventSize;
	typedef vector<struct epoll_event> EventList;
	typedef map<int, Channel *> ChannelMap;
	EventList epollEvents_;
	int epollFd_;
	ChannelMap channels_;
};
}
#endif
