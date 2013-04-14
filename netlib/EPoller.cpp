#include "Channel.h"
#include "EPoller.h"
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace mynet;

const int EPoller::MaxEventSize = 1024;
EPoller::EPoller()
{
	epollEvents_.resize(MaxEventSize);
	epollFd_ = ::epoll_create(MaxEventSize);
}

EPoller::~EPoller()
{
	::close(epollFd_);
}

void EPoller::update(int op, Channel *channel)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(struct epoll_event));
	if (channel->getEvent() & Channel::kReadEvent) {
		event.events |= EPOLLIN;
	} 
	if (channel->getEvent() & Channel::kWriteEvent) {
		event.events |= EPOLLOUT;
	}
	event.data.ptr = (void *)channel;
	
	if (event.events & EPOLLIN) {
//		printf("EPOLLIN\n");
//		printf("%d\n", channel->fd());
	}
	if (event.events & EPOLLOUT) {
//		printf("EPOLLOUT\n");
	}
	if (::epoll_ctl(epollFd_, op, channel->fd(), &event) < 0) {
		perror("epoll_ctl error");
	}
}

void EPoller::addChannel(Channel *channel)
{
	int fd = channel->fd();
	printf("%d\n", channel->fd());
	assert(channels_.find(fd) == channels_.end());
	
	update(EPOLL_CTL_ADD, channel);
	channels_[fd] = channel;
}

void EPoller::updateChannel(Channel *channel)
{
	int fd = channel->fd();
	assert(channels_[fd] == channel);
	update(EPOLL_CTL_MOD, channel);
}

void EPoller::removeChannel(Channel *channel)
{
	int fd = channel->fd();
	assert(channels_[fd] == channel);
	channels_.erase(fd);
	update(EPOLL_CTL_DEL, channel);
}

int EPoller::epollToEvent(int epollEvent)
{
	int event = 0;
	if (epollEvent & EPOLLIN) {
		event |= Channel::kReadEvent;
	}
	if (epollEvent & EPOLLOUT) {
		event |= Channel::kWriteEvent;
	}
	return event;
}
void EPoller::fillActiveChannels(int nEvents, ChannelList &activeChannels)
{
	for (int i = 0; i < nEvents; i++) {
		Channel *channel = static_cast<Channel *>(epollEvents_[i].data.ptr);
		channel->setRevent(epollToEvent(epollEvents_[i].events));
		activeChannels.push_back(channel);
		assert(channels_[channel->fd()] == channel);
	}
}

void EPoller::poll(int timeout, ChannelList &activeChannels)
{
	int nEvents = ::epoll_wait(epollFd_, 
				&epollEvents_[0], 
				MaxEventSize,
				timeout);
	if (nEvents > 0) {
		//printf("%d events happens\n", nEvents);
		fillActiveChannels(nEvents, activeChannels);
	} else if (nEvents == 0) {
//		printf("Nothing happen\n");
	} else {
		perror("poll error\n");
	}
}
