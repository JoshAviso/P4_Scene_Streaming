#pragma once

#include <SceneStreamClient.h>

#include <iostream>

void RunClient(const String& name, const String& socket_address, bool waitForInput) {
	SceneStreamClient client(
		grpc::CreateChannel(socket_address, grpc::InsecureChannelCredentials()));

	client.PingHello(name);

	int x = 0;
	if (waitForInput) 
		std::cin >> x;
}