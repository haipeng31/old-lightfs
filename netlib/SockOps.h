#ifndef MYNET_SOCKOPS_H
#define MYNET_SOCKOPS_H

#include <stdio.h>
namespace mynet {

int socket();
void bind(int, int);
void listen(int, int);
int accept(int, char * = NULL, int * = NULL); 
int connect(int, const char *, int);
void setNonBlocking(int sockfd);
}
#endif
