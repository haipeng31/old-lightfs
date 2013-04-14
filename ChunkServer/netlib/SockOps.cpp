#include "SockOps.h"
#include "base/Logging.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int mynet::socket()
{
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		LOG_ERROR << "create socket error";
	}

	return fd;
}

void mynet::bind(int listenfd, int port)
{
	struct sockaddr_in serverAddr;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons((short)port);
	if (::bind(listenfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		LOG_ERROR << "bind error";
	}
}

void mynet::listen(int listenfd, int backlog)
{
	if (::listen(listenfd, backlog) < 0) {
		LOG_ERROR << "listen error";
	}
}

int mynet::accept(int listenfd, char *peerAddr, int *peerPort)
{
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int clilen = sizeof(clientAddr);

	int fd = ::accept(listenfd, (struct sockaddr *)&clientAddr, (socklen_t *)&clilen);
	
	if (fd < 0) {
		LOG_ERROR << "accept error";
	}
	if ((peerAddr != NULL) && (peerPort != NULL)) {
		strcpy(peerAddr, inet_ntoa(clientAddr.sin_addr));
		*peerPort = (int)ntohs(clientAddr.sin_port);
	}
	
	return fd;
}

int mynet::connect(int sock, const char *ipAddr, int port)
{
	struct sockaddr_in serverAddr;

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((short)port);
	serverAddr.sin_addr.s_addr = inet_addr(ipAddr);

	if (::connect(sock, (struct sockaddr *)&serverAddr, sizeof(sockaddr_in)) < 0) {
		LOG_ERROR << "connect error";
		return -1;
	}
	return 0;
}

void mynet::setNonBlocking(int sockfd)
{
	int flags = ::fcntl(sockfd, F_GETFL, 0);
	if (flags < 0) {
		LOG_ERROR << "get flag error on fd " << sockfd;
		return;
	}

	flags = O_NONBLOCK;

	int ret = ::fcntl(sockfd, F_SETFL, flags);
	if (ret < 0) {
		LOG_ERROR << "set flag error on fd " << sockfd;
	}
}
