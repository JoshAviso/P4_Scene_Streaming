#pragma once

#include <grpcpp/grpcpp.h>

#include "../src_common/Common.h"
#include "../src_common/String.h"

#include "../proto/generated/SceneStreaming.grpc.pb.h"

class SceneStreamClient {
private:
	Unique<SceneStreamService::Stub> _stub;

public:
	SceneStreamClient(Shared<grpc::Channel> channel)
		: _stub(SceneStreamService::NewStub(channel)) {};

	void PingHello(const String& name);
};