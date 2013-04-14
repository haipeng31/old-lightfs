#include "Buffer.h"
#include "base/Logging.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

using namespace mynet;

const int Buffer::kInitSize = 128;

Buffer::Buffer(int size) 
  : size_(size),
	off_(0)
{
	if ((buffer_ = (char *)malloc(size * sizeof(char))) == NULL) {
		LOG_ERROR << "malloc error";
	}
	oriBuffer_ = buffer_;
	checkState();
}

Buffer::~Buffer()
{
	assert(oriBuffer_ != NULL);
	free(oriBuffer_);
}

void Buffer::checkState()
{
	LOG_TRACE << "off_ = " << off_ << " size_ = " << size_ << " buffer_ - oriBuffer_ = " << buffer_ - oriBuffer_;
		
}
int Buffer::readBuf(char *dest, int len)
{
	if (len <= off_) {
		memcpy(dest, buffer_, len);
		buffer_ += len;
		off_ -= len;
		return len;
	} else {
		int temp = off_;
		memcpy(dest, buffer_, off_);
		buffer_ += off_;
		off_ = 0;
		return temp;
	}
}

void Buffer::writeBuf(const char *src, int len)
{
	LOG_TRACE << "Write Buffer";
	if ((size_ - off_ - (buffer_ - oriBuffer_)) > len) {
		LOG_TRACE << "-1";
		appendBuf(src, len);
	} else {
		LOG_TRACE << "0";
		LOG_TRACE << "off_ = " << off_ << " size_ = " << size_ << " buffer_ - oriBuffer_ = " << buffer_ - oriBuffer_ << " len = " << len;
		memcpy(oriBuffer_, buffer_, off_);
		buffer_ = oriBuffer_;
		LOG_TRACE << "1";
		if (size_ - off_ > len) {
			LOG_TRACE << "2";
			appendBuf(src, len);
		} else {
			LOG_TRACE << "3";
			expandBuf(len);
			appendBuf(src, len);
		}
		LOG_TRACE << "4";
	}
	LOG_TRACE << "Write Buffer End";
}

/* just use it in writeBuf, other func should
 * use writeBuf 
 */
void Buffer::appendBuf(const char *src, int len)
{
	memcpy(buffer_ + off_, src, len);
	off_ += len;
}

void Buffer::expandBuf(int len)
{
	int need = len + off_;
	if (need < kInitSize) {
		need = kInitSize;
	}
	while (size_ < need) {
		size_ <<= 1;
	}

	buffer_ = (char *)realloc(buffer_, size_);
	if (buffer_ == NULL) {
		LOG_ERROR << "realloc error";
	}

	oriBuffer_ = buffer_;
}

int Buffer::bufReadFd(int fd)
{
	char inbuf[4096];

	int n = ::read(fd, inbuf, sizeof(inbuf));
	if (n == 0) {
		return 0;
	} else if (n == -1) {
		LOG_ERROR << "read error";
		return -1;
	} else {
		writeBuf(inbuf, n);
		return n;
	}	
}

int Buffer::bufWriteFd(int fd, int len)
{
	if (len > off_) {
		len = off_;
	}
	int n = ::write(fd, buffer_, len);
	if (n == 0) {
		return 0;
	} else if (n == -1) {
		LOG_ERROR << "write error";
		return -1;
	} else {
		buffer_ += n;
		off_ -= n;
		return n;
	}
}

int Buffer::avail()
{
	return off_;
}

void Buffer::showContent()
{
	char content[1024];
	memset(content, 0, sizeof(content));
	strncpy(content, buffer_, off_);
	LOG_TRACE << content;
}
