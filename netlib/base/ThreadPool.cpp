#include "ThreadPool.h"
#include "Logging.h"
#include "assert.h"

using namespace mynet;

ThreadPool::ThreadPool(int nthread)
 :  taskQueue_(),
	mutex_(),
	condition_(mutex_),
	threads_(nthread),
	stop_(false)
{
}

void ThreadPool::start()
{
	int rt;
	for (int i = 0; i < threads_.size(); i++) {
		 rt = pthread_create(&threads_[i], NULL, threadFunc, this);
		 if (rt < 0) {
			 LOG_ERROR << "create thread error";
		 }
	}
	
	LOG_TRACE << "ThreadPool initialized";
}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::destroy()
{
	stop_ = true;
	condition_.notifyAll();
	for (int i = 0; i < threads_.size(); i++) {
		pthread_join(threads_[i], NULL);
	}
	
	LOG_TRACE << "TreadPool Destroyed";
}

void ThreadPool::putTask(Task *task)
{
	assert(task != NULL);

	MutexLockGuard mutexLockGuard(mutex_);
	
	if (!stop_) {
		taskQueue_.push(task);
		condition_.notify();
	} else {
		LOG_INFO << "ThreadPool is stopping, don't add new tasks";
	}
}

Task *ThreadPool::getTask()
{
	Task *task = NULL;
	MutexLockGuard mutexLockGuard(mutex_);

	while ((!stop_) && (taskQueue_.size() == 0)) {
		condition_.wait();
	}
	
	if (taskQueue_.size() != 0) {
		task = taskQueue_.front();
		taskQueue_.pop();
	}
	
	return task;
}

void *ThreadPool::threadFunc(void *args)
{
	ThreadPool *pool = (ThreadPool *)args;
	Task *task;
	while (task = pool->getTask()) {
		task->taskFunc(task->objArg, task->args);
		delete task;
	}
}
