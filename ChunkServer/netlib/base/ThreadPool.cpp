#include "ThreadPool.h"
#include "Logging.h"
#include "assert.h"
#include <string>
#include "../../FileSystem.pb.h"      // for WriteReq
#include "../Codec.h"      //for TaskFuncArg
using std::string;
using myfs::WriteReq;
using namespace mynet;

ThreadPool::ThreadPool(int nthread, int nwriteThread)
 :  taskQueue_(),
	mutex_(),
	condition_(mutex_),
	threads_(nthread),
	writeTaskQueues_(nwriteThread),
	writeThreads_(nwriteThread),
	stop_(false)
{
	for (int i = 0; i < nwriteThread; i++) {
		MutexLock *tempMutex = new MutexLock;
		Condition *tempCondition = new Condition(*tempMutex);
		writeMutexs_.push_back(tempMutex);
		writeConditions_.push_back(tempCondition);
	}
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
	
	for (int i = 0; i < writeThreads_.size(); i++) {
		WriteThreadArgs *writeThreadArgs = new WriteThreadArgs;
		writeThreadArgs->pool = this;
		writeThreadArgs->queueIndex = i;
		rt = pthread_create(&writeThreads_[i], NULL, writeThreadFunc, writeThreadArgs);
		if (rt < 0) {
			LOG_ERROR << "create write thread error";
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

	for (int i = 0; i < writeThreads_.size(); i++) {
		delete writeConditions_[i];
		delete writeMutexs_[i];
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

void ThreadPool::putWriteTask(Task *writeTask)
{
	assert(writeTask != NULL);
	
	TaskFuncArg *taskFuncArg = (TaskFuncArg *)(writeTask->args);
	string reqByteStream((const char *)taskFuncArg->msg, taskFuncArg->length); 
	WriteReq writeReq;
	writeReq.ParseFromString(reqByteStream);

	// hash chunkid to index
	int index = writeReq.chunkid() % (long long)writeThreads_.size();

	MutexLockGuard mutexLockGuard(*writeMutexs_[index]);
	
	if (!stop_) {
		writeTaskQueues_[index].push(writeTask);
		writeConditions_[index]->notify();
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

Task *ThreadPool::getWriteTask(int index)
{
	Task *task = NULL;
	MutexLockGuard mutexLockGuard(*writeMutexs_[index]);

	while ((!stop_) && (writeTaskQueues_[index].size() == 0)) {
		writeConditions_[index]->wait();
	}
	
	if (writeTaskQueues_[index].size() != 0) {
		task = writeTaskQueues_[index].front();
		writeTaskQueues_[index].pop();
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

void *ThreadPool::writeThreadFunc(void *args)
{
	WriteThreadArgs *writeThreadArgs = (WriteThreadArgs *)args;
	ThreadPool *pool = writeThreadArgs->pool;
	int index = writeThreadArgs->queueIndex;
	Task *task;
	while (task = pool->getWriteTask(index)) {
		task->taskFunc(task->objArg, task->args);
		delete task;
	}
	
	delete writeThreadArgs;
}
