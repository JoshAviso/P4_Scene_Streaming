#pragma once

#include <Threading/ThreadPool.h>
#include <Common.hpp>
#include <String.hpp>

#include <Threading/IThreadFinishedCallback.h>

class ThreadPoolManager {
private:
	Dict<String, Shared<ThreadPool>> _threadPools;

public:
	static Shared<ThreadPool> NewThreadPool(String name, uint threadCount, IThreadFinishedCallback* callback = nullptr);
	static Shared<ThreadPool> GetThreadPool(String name);

	// SINGLETON
public:
	static ThreadPoolManager* Initialize();
private:
	static ThreadPoolManager* _instance;
	ThreadPoolManager() = default;
	~ThreadPoolManager() = default;
	ThreadPoolManager(const ThreadPoolManager&) = delete;
	ThreadPoolManager& operator= (const ThreadPoolManager&) = delete;

};