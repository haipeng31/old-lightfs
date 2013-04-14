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
	ThreadPool(int nthread, int nwriteThread);
	~ThreadPool();
	
	void start();
	void destroy();
	void putTask(Task *task);
	void putWriteTask(Task *writeTask);
private:
	static void *threadFunc(void *args);
	static void *writeThreadFunc(void *args);
	Task *getTask();
	Task *getWriteTask(int index);
	queue<Task *> taskQueue_;
	MutexLock mutex_;
	Condition condition_;
	vector<pthread_t> threads_;
	vector<queue<Task *> > writeTaskQueues_;
	vector<MutexLock *> writeMutexs_;
	vector<Condition *> writeConditions_;
	vector<pthread_t> writeThreads_;

	bool stop_;
};

struct WriteThreadArgs {
	ThreadPool *pool;
	int queueIndex;
};

}
#endif
