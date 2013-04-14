#include "Channel.h"
#include <iostream>
#include <stdio.h>
#include "base/Logging.h"
using namespace mynet;

const int Channel::kReadEvent = 0x1;
const int Channel::kWriteEvent = 0x2;
const int Channel::kNoEvent = 0x0;


Channel::Channel(int fd)
  :	fd_(fd),
	events_(kNoEvent),
	revents_(kNoEvent),
	readCallback_(NULL),
	writeCallback_(NULL),
	closeCallback_(NULL),
	errorCallback_(NULL)
{
}

void Channel::setEvent(int event)
{
	events_ |= event;
}

void Channel::dissetEvent(int event)
{
	events_ &= ~event;
}

int Channel::getEvent()
{
	return events_;
}

void Channel::setRevent(int revents)
{
	revents_ = revents;
}

void Channel::handleEvent()
{
	if (revents_ & kReadEvent) {
#ifdef TRACE
		LOG_TRACE << "handle read";
#endif
		readCallback_(args_);
	}
	if (revents_ & kWriteEvent) {
		printf("handlewrite in Channel\n");
		writeCallback_(args_);
	}
}

int Channel::fd()
{
	return fd_;
}

void Channel::setReadCallback(Callback readCallback)
{
	readCallback_ = readCallback;
}

void Channel::setWriteCallback(Callback writeCallback)
{
	writeCallback_ = writeCallback;
}

void Channel::setCloseCallback(Callback closeCallback)
{
	closeCallback_ = closeCallback;
}

void Channel::setErrorCallback(Callback errorCallback)
{
	errorCallback_ = errorCallback;
}

void Channel::setArgs(void *args)
{
	args_ = args;
}

void *Channel::args()
{
	return args_;
}
