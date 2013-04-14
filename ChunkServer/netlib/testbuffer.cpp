#include "Buffer.h"
#include "base/Logging.h"
#include <string.h>
#include <stdlib.h>

using namespace mynet;

/*
int main()
{
	Buffer buffer(8);
	char teststr[10] = "123456789";

	for (int i = 0; i < 20; i++) {
		buffer.writeBuf(teststr, 9);
	}
	LOG_INFO << "buffer avail" << ' ' << buffer.avail();
	char temp[201];
	memset(temp, 0, sizeof(temp));
	buffer.readBuf(temp, buffer.avail());
	temp[200] = '\0';
	LOG_INFO << temp;
	LOG_INFO << "buffer avail" << ' ' << buffer.avail();
	return 0;
}
*/
