#include "SockOps.h"
#include "base/Logging.h"
#include <stdlib.h>
#include <string.h>

int main()
{
	int sockfd = mynet::socket();
	mynet::bind(sockfd, 5555);
	mynet::listen(sockfd, 10);

	while (1) {
		char clientAddr[20];
		int clientPort;

		memset(clientAddr, 0, sizeof(clientAddr));
		clientPort = 0;

		int clientfd = mynet::accept(sockfd, clientAddr, &clientPort);

		mynet::LOG_INFO << clientAddr << ' ' << clientPort;
	}

	return 0;
}
