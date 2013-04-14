#ifndef POLLER_H
#define POLLER_H

#include <vector>
using std::vector;

namespace mynet {

class Channel;

class Poller {
public:
	typedef vector<Channel *> ChannelList;
	Poller();
	virtual ~Poller();

	virtual void poll(int timeout, ChannelList &) = 0;
	
	virtual void addChannel(Channel *) = 0;
	virtual void updateChannel(Channel *) = 0;
	virtual void removeChannel(Channel *) = 0;

};

}
#endif
