#pragma once

#include <grpcpp/grpcpp.h>
#include <SceneStreaming.grpc.pb.h>

class SceneStreamServer final : public SceneStreamService::Service {
public:
	grpc::Status GetSceneObjectData(grpc::ServerContext* context, const SceneReq* request, grpc::ServerWriter<ObjectReply>* writer) override;
	grpc::Status GetSceneList(grpc::ServerContext* context, const Empty* request, SceneListReply* reply) override;
	grpc::Status AskSceneInfo(grpc::ServerContext* context, const SceneReq* request, SceneInfoReply* reply) override;

private:
	bool fileExists(std::string fileName);

//Singleton
public:
	static SceneStreamServer& Instance() {
		static SceneStreamServer instance;
		return instance;
	}
private:
	SceneStreamServer() {};
	~SceneStreamServer() {};

	SceneStreamServer(const SceneStreamServer&) = delete;
	SceneStreamServer& operator=(const SceneStreamServer&) = delete;
};