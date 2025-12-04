#pragma once

#include <grpcpp/grpcpp.h>
#include <SceneStreaming.grpc.pb.h>

#include <Logger.hpp>
#include <String.hpp>
#include <Common.hpp>

class SceneStreamServer final : public SceneStreamService::Service {
public:
	grpc::Status GetSceneObjectData(grpc::ServerContext* context, const SceneReq* request, grpc::ServerWriter<ObjectBatchReply>* writer) override;
	grpc::Status GetSceneList(grpc::ServerContext* context, const Empty* request, SceneListReply* reply) override;
	grpc::Status AskSceneInfo(grpc::ServerContext* context, const SceneReq* request, SceneInfoReply* reply) override;

private:
	bool fileExists(std::string fileName);

//Singleton
public:
	static SceneStreamServer* instance;
	bool stopServer = false;
	SceneStreamServer() {};
	~SceneStreamServer() {};
private:

	SceneStreamServer(const SceneStreamServer&) = delete;
	SceneStreamServer& operator=(const SceneStreamServer&) = delete;
};

void RunServer(const String socket_address);
