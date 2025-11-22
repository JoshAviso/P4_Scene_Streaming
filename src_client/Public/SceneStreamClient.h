#pragma once

#include <grpcpp/grpcpp.h>

#include <Common.hpp>
#include <String.hpp>

#include <SceneStreaming.grpc.pb.h>

class SceneStreamClient {
private:
	Unique<SceneStreamService::Stub> _stub;

public:
	SceneStreamClient(Shared<grpc::Channel> channel)
		: _stub(SceneStreamService::NewStub(channel)) {};

	void PingHello(const String& name);
};