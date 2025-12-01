#include <Threading/WorkerThread.h>

WorkerThread::WorkerThread(int id, IThreadFinishedCallback* callback)
 : _id(id), _onFinished(callback), _task(nullptr) {}

void WorkerThread::Run()
{
	if (_task != nullptr) {
		_task->DoWorkerTask(_id);
		delete _task;
		_task = nullptr;
	}
	
	if (_onFinished != nullptr) 
		_onFinished->OnThreadFinished(_id);
}
