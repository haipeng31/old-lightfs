#include "SockOps.h"
#include "base/Logging.h"
#include <stdio.h>

int main()
{
	int sockfd = mynet::socket();
	mynet::connect(sockfd, "127.0.0.1", 5555);
	mynet::LOG_INFO << "connect";
	int i;
	scanf("%d", &i);
}
