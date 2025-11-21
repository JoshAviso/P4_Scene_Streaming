
#pragma once

#include <grpcpp/grpcpp.h>

#include "src_common/String.h"
#include "src_common/Common.h"
#include "src_common/Logger.h"

#include "src_server/SceneStreamServer.h"

void RunServer(String socket_address) {
	SceneStreamServer service;

	grpc::ServerBuilder builder;
	builder.AddListeningPort(socket_address, grpc::InsecureServerCredentials())
		.RegisterService(&service);

	Unique<grpc::Server> server(builder.BuildAndStart());
	Logger::Log("Server Listening at: " + socket_address);

	server->Wait();
}