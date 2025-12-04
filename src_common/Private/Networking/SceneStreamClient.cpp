#include <Networking/SceneStreamClient.h>

#include <Logger.hpp>
#include <../Public/Scenes/SceneManager.h>
#include <../Public/Math/TempObject.h>

std::vector<std::string> SceneStreamClient::GetSceneList()
{
	Empty request;

	SceneListReply response;
	grpc::ClientContext context;

	grpc::Status status =
		_stub->GetSceneList(&context, request, &response);

	if (status.ok()) {
		Logger::Log("SceneList received");

		int byteSize = response.ByteSize();
		std::string* sceneNames = new std::string[byteSize];

		//sceneNames array should have the scenes?
		response.SerializeToArray(sceneNames, byteSize);

		//array to pointer
		size_t size = 5;

		std::vector<std::string> vectorNames(sceneNames, sceneNames + size);

		delete[] sceneNames;
		sceneNames = nullptr;

		return vectorNames;
	}
	else {
		Logger::LogWarning("Server call to GetSceneList failed.");
	}
}

void SceneStreamClient::AskSceneInfo(bool* exists, int* totalObjects, std::string sceneName)
{
	SceneReq request;
	request.set_name(sceneName);

	SceneInfoReply response;
	grpc::ClientContext context;

	grpc::Status status =
		_stub->AskSceneInfo(&context, request, &response);

	if (status.ok()) {
		Logger::Log("SceneInfo received");

		*exists = response.status();
		*totalObjects = response.totalobjects();
	}
	else {
		Logger::LogWarning("Server call to AskSceneInfo failed");
	}
}

SceneObjectsReply SceneStreamClient::GetSceneObjectData(std::string sceneName)
{
	std::vector<TempObject> loadedObjects;

	SceneReq request;
	request.set_name(sceneName);

	grpc::ClientContext context;
	std::shared_ptr<grpc::ClientReader<SceneObjectsReply>> reader =
		_stub->GetSceneObjectData(&context, request);

	SceneObjectsReply reply;
	while (reader->Read(&reply))
	{
		for (const auto& obj : reply.objects())
		{
			TempObject temp;

			temp.position = {
				obj.position().x(), obj.position().y(), obj.position().z()
			};

			temp.rotation = {
				obj.rotation().x(), obj.rotation().y(), obj.rotation().z()
			};

			temp.scale = {
				obj.scale().x(), obj.scale().y(), obj.scale().z()
			};

			for (const auto& v : obj.vertices())
			{
				TempVertex vert;
				vert.position = {
					v.position().x(), v.position().y(), v.position().z()
				};
				vert.normal = {
					v.normal().x(), v.normal().y(), v.normal().z()
				};
				vert.uv = {
					v.uv().x(), v.uv().y()
				};

				temp.vertices.push_back(vert);
			}

			loadedObjects.push_back(std::move(temp));
		}
	}

	grpc::Status status = reader->Finish();
	if (status.ok()) Logger::Log("Object Streaming succeeded.");
	else Logger::LogWarning("Object streaming failed.");
}
