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

	std::vector<std::string> GetSceneList();
	void AskSceneInfo(bool* exists, int* totalObjects, std::string sceneName);
	SceneObjectsReply GetSceneObjectData(std::string sceneName);

//Singleton
public:
	static SceneStreamClient& Instance() {
		static SceneStreamClient instance;
		return instance;
	};

private:
	SceneStreamClient() {};
	~SceneStreamClient() {};

	SceneStreamClient(const SceneStreamClient&) = delete;
	SceneStreamClient& operator=(const SceneStreamClient&) = delete;
};