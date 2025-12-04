#pragma once

#include <grpcpp/grpcpp.h>

#include <Common.hpp>
#include <String.hpp>

#include <SceneStreaming.grpc.pb.h>

class SceneStreamClient {
public:
	Unique<SceneStreamService::Stub> _stub;

public:

	std::vector<std::string> GetSceneList();
	void AskSceneInfo(bool* exists, int* totalObjects, std::string sceneName);

//Singleton
public:
	static SceneStreamClient* instance;
	static SceneStreamClient* Instance() {
		return instance;
	}
	static void Initialize(const String& socket_address) {
		instance = new SceneStreamClient(socket_address);
	};

private:
	SceneStreamClient(const String& socket_address) {
		Shared<grpc::Channel> chnl = grpc::CreateChannel(socket_address, grpc::InsecureChannelCredentials());
		_stub = SceneStreamService::NewStub(chnl);
	}
	~SceneStreamClient() = default;

	SceneStreamClient(const SceneStreamClient&) = delete;
	SceneStreamClient& operator=(const SceneStreamClient&) = delete;
};