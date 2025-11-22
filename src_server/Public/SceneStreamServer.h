#pragma once

#include <SceneStreaming.grpc.pb.h>

class SceneStreamServer final : public SceneStreamService::Service {
public:
	grpc::Status PingHello(grpc::ServerContext* context, const HelloReq* request, HelloReply* reply) override;
};