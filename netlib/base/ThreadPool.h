#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Mutex.h"
#include "Condition.h"
#include <queue>
#include <vector>
#include <pthread.h>

using std::queue;
using std::vector;
namespace mynet {

struct Task {
	void (*taskFunc)(void *objArg, void *args);
	void *objArg;
	void *args;
};

class ThreadPool {
public:
	ThreadPool(int nthread);
	~ThreadPool();
	
	void start();
	void destroy();
	void putTask(Task *task);
private:
	static void *threadFunc(void *args);
	Task *getTask();
	queue<Task *> taskQueue_;
	MutexLock mutex_;
	Condition condition_;
	vector<pthread_t> threads_;
	bool stop_;
};
}
#endif
