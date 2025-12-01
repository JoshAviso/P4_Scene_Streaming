#pragma once

#include <Common.hpp>
#include <Threading/IThread.h>
#include <Threading/WorkerThread.h>
#include <Threading/IThreadFinishedCallback.h>
#include <Threading/WorkerTasks/IWorkerTask.h>

class ThreadPool : public IThread, public IThreadFinishedCallback
{
public:
	bool StopWhenCompleted = false;

private:
	uint _workerCount;
	bool _isRunning = false;

	Queue<IWorkerTask*> _pendingTasks;
	Map<int, WorkerThread*> _activeThreads;
	Queue<WorkerThread*> _idleThreads;
	IThreadFinishedCallback* _onFinishCallback = nullptr;

public:
	ThreadPool(uint threadCount, IThreadFinishedCallback* onFinish = nullptr);
	virtual ~ThreadPool() = default;

	void ScheduleTask(IWorkerTask* task);
	void SetOnFinishCallback(IThreadFinishedCallback* callback);

	void StartScheduling();
	void StopScheduling();

private:
	void Run() override;
	void OnThreadFinished(int id) override;
};
