#include <SceneStreamServer.h>

#include <Logger.hpp>

grpc::Status SceneStreamServer::PingHello(grpc::ServerContext* context, const HelloReq* request, HelloReply* reply)
{
	Logger::Log("Received Message from " + request->name());

	reply->set_status(true);
	reply->set_msg("Hello " + request->name() + "!\n");

	return grpc::Status::OK;
}
