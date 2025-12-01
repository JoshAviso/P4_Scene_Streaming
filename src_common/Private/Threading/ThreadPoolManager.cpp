#include <Threading/ThreadPoolManager.h>

#include <Logger.hpp>

Shared<ThreadPool> ThreadPoolManager::NewThreadPool(String name, uint threadCount, IThreadFinishedCallback* callback)
{
	if (_instance->_threadPools.find(name) != _instance->_threadPools.end()) {
		return _instance->_threadPools[name];
	}

	Shared<ThreadPool> pool = Make_Shared<ThreadPool>(threadCount, callback);
	_instance->_threadPools[name] = pool;

	return pool;
}

Shared<ThreadPool> ThreadPoolManager::GetThreadPool(String name)
{
	if (_instance->_threadPools.find(name) == _instance->_threadPools.end()) {
		Logger::LogWarning("Trying to get non-existent thread pool: " + name);
		return nullptr;
	}
	return _instance->_threadPools[name];
}

ThreadPoolManager* ThreadPoolManager::_instance = nullptr;
ThreadPoolManager* ThreadPoolManager::Initialize()
{
	if (_instance == nullptr) _instance = new ThreadPoolManager();
	return _instance;
}
