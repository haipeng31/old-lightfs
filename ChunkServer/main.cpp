#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ChunkServer.h"

using namespace myfs;

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage:chunkserver port dir\n");
		exit(0);
	}

	int port;
	sscanf(argv[1], "%d", &port);
	ChunkServer chunkServer("ChunkServer", "127.0.0.1", port, 5, 5, argv[2], 100);

	chunkServer.start();

	return 0;
}
