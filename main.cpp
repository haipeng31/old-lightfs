#include "MetaServer.h"
using namespace myfs;

int main()
{
	MetaServer metaServer("MetaServer", "127.0.0.1", 5555, 10);
	metaServer.start();
	return 0;
}

