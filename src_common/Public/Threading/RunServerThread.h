#pragma once

#include <Networking/SceneStreamServer.h>
#include <Threading/IThread.h>

#include <String.hpp>

class RunServerThread : public IThread {
private:
	String _socket_address;

public:
	RunServerThread(const String& socket_address) : _socket_address(socket_address) {};
	void Run() override {
		RunServer(_socket_address);
	}
};