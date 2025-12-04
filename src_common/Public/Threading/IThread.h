#pragma once

#include <thread>
#include <Common.hpp>

class IThread
{
public:
	IThread() = default;
	virtual ~IThread() = default;

	void StartThread() { std::thread(&IThread::Run, this).detach(); } ;
	static void Sleep(uint ms) 
		{ std::this_thread::sleep_for(std::chrono::milliseconds(ms)); };

public:
	virtual void Run() = 0;
};

