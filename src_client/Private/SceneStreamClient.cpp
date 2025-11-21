#include "../SceneStreamClient.h"

#include "../../src_common/Logger.h"

void SceneStreamClient::PingHello(const String& name)
{
	HelloReq req;
	req.set_name(name);
	HelloReply reply;
	grpc::ClientContext context;

	grpc::Status status =
		_stub->PingHello(&context, req, &reply);

	if (status.ok()) Logger::Log("Message(" + ToString(reply.status()) + "): " + reply.msg());
	else Logger::LogWarning("Server call failed!");
}
