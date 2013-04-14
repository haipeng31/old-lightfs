#ifndef BUFFER_H
#define BUFFER_H

namespace mynet {

class Buffer {
public:
	Buffer(int size = kInitSize);
	~Buffer();
	
	void checkState();
	int readBuf(char *, int);
	void writeBuf(const char *, int);
	int bufReadFd(int);
	int bufWriteFd(int, int);
	int avail();
	void showContent();
private:
	static const int kInitSize;
	void expandBuf(int);
	void appendBuf(const char *src, int len);
	char *oriBuffer_;
	char *buffer_;
	int off_;
	int size_;
};

}
#endif
