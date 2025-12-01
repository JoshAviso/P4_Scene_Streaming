#pragma once

#include <Threading/IThread.h>
#include <Threading/WorkerTasks/IWorkerTask.h>
#include <Threading/IThreadFinishedCallback.h>

class WorkerThread : public IThread
{
private:
	int _id;
	IThreadFinishedCallback* _onFinished;
	IWorkerTask* _task;

public:
	WorkerThread(int id, IThreadFinishedCallback* callback);
	~WorkerThread() = default;

	void AssignTask(IWorkerTask* task) { this->_task = task; }
	int GetID() const { return _id; }

private:
	void Run() override;
};