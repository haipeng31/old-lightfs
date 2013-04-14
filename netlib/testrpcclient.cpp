#include "EchoClient.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace mynet;
int main()
{
	EchoClient echoClient("Echo-Client", "127.0.0.1", 5555);
	char str[10];
	while (1) {
		memset(str, 0, sizeof(str));
		scanf("%s", str);
		printf("%s\n", echoClient.echo(str));
	}

	return 0;
}
