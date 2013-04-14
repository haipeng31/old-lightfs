#include "Logging.h"
#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include "Mutex.h"

using namespace mynet;

int fd;
char buf[10241];
struct timeval start, end;
int count = 1024;
MutexLock mutexLock;

void timediff(struct timeval start, struct timeval end)
{
	if (end.tv_usec < start.tv_usec) {
		end.tv_usec += 1000000;
		end.tv_sec -= 1;
	}

	printf("cost time %ds and %dms\n", end.tv_sec-start.tv_sec, end.tv_usec-start.tv_usec);
}
void test(void *objarg, void *args)
{
	int i = (int)args;
	
	lseek(fd, i * 10240, SEEK_SET);
	write(fd, buf, 10240);
	//std::cout << "In task " << i << std::endl;
	
	mutexLock.lock();
	count--;
	if (count == 0) {
		gettimeofday(&end, NULL);
		timediff(start, end);
	}
	mutexLock.unlock();
	//printf("%d ", i);
}

int main()
{
	ThreadPool pool(10);
	pool.start();

	fd = open("test.out", O_WRONLY);
	for (int i = 0; i < 10241; i++) {
		buf[i] = 'a';
	}
	buf[10240] = 0;
	gettimeofday(&start, NULL);
	for (int i = 0; i < 1024; i++) {
		Task *task = new Task;
		task->taskFunc = test;
		task->args = (void *)i;

		pool.putTask(task);
	}
	std::cout << "finish put task" << std::endl;

	sleep(2);
	close(fd);

	pool.destroy();
	return 0;
}
