#include <Threading/ThreadPool.h>

#include <Logger.hpp>

ThreadPool::ThreadPool(uint threadCount, IThreadFinishedCallback* onFinish) 
	: _workerCount(threadCount), _onFinishCallback(onFinish)
{
	int count = (int)_workerCount;
	for (int i = 0; i < count; i++) {
		_idleThreads.push(new WorkerThread(i, this));
	}
}

void ThreadPool::ScheduleTask(IWorkerTask* task)
	{ _pendingTasks.push(task); }

void ThreadPool::SetOnFinishCallback(IThreadFinishedCallback * callback)
	{ _onFinishCallback = callback; }

void ThreadPool::StartScheduling()
{
	_isRunning = true;
	this->StartThread();
}

void ThreadPool::StopScheduling()
{
	_isRunning = false;
}

void ThreadPool::Run()
{
	while (_isRunning) {
		// If it has pending tasks
		if (!_pendingTasks.empty()) {
			// If there are idle threads
			if (!_idleThreads.empty()) {
				// Take the first idle thred
				WorkerThread* worker = _idleThreads.front();
				_idleThreads.pop();

				// Assign it as an active thread
				_activeThreads[worker->GetID()] = worker;

				// Assign it the first queued task and start
				IWorkerTask* task = _pendingTasks.front();
				_pendingTasks.pop();
				worker->AssignTask(task);
				worker->StartThread();
			}
		}
		// Once StopWhenCompleted is true and there are no pending tasks, stop the pool and call the finish callback
		else if (StopWhenCompleted && _activeThreads.empty()) {
			_isRunning = false;
			if (_onFinishCallback != nullptr) {
				_onFinishCallback->OnThreadFinished(-1);
			}
		}
	}
}

void ThreadPool::OnThreadFinished(int id)
{
	if (_activeThreads[id] != nullptr) {
		delete _activeThreads[id];
		_activeThreads.erase(id);

		_idleThreads.push(new WorkerThread(id, this));
	}
}
