#pragma once

#include <grpcpp/grpcpp.h>
#include <SceneStreaming.grpc.pb.h>

class SceneStreamServer final : public SceneStreamService::Service {
public:
	grpc::Status PingHello(grpc::ServerContext* context, const HelloReq* request, HelloReply* reply) override;
	grpc::Status SendObject(grpc::ServerContext* context, const ObjectReq* request, ObjectReply* reply) override;

private:
	bool fileExists(std::string fileName);
};