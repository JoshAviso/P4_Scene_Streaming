#include <SceneStreamServer.h>

#include <Logger.hpp>
#include <fstream>

grpc::Status SceneStreamServer::PingHello(grpc::ServerContext* context, const HelloReq* request, HelloReply* reply)
{
	Logger::Log("Received Message from " + request->name());

	reply->set_status(true);
	reply->set_msg("Hello " + request->name() + "!\n");

	return grpc::Status::OK;
}

grpc::Status SceneStreamServer::SendObject(grpc::ServerContext* context, const ObjectReq* request, ObjectReply* reply)
{
	Logger::Log("Sending object data for " + request->name());

	if (fileExists(request->name()))
	{
		std::cout << "Kill me, Emmanuel John 'EJ' R. Taylan." << std::endl;
	}
}

bool SceneStreamServer::fileExists(std::string fileName)
{
	std::ifstream file(fileName);
	return file.good();
}
