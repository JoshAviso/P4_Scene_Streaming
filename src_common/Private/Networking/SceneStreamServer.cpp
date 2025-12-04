#include <Networking/SceneStreamServer.h>

#include <Logger.hpp>
#include <fstream>
#include <../Public/Scenes/SceneManager.h>
#include <../Public/Components/Renderers/MeshRenderer.h>

grpc::Status SceneStreamServer::GetSceneObjectData(grpc::ServerContext* context, const SceneReq* request, grpc::ServerWriter<ObjectReply>* writer)
{
	Logger::Log("Sending scene data for " + request->name());

	Shared<Scene> scene = SceneManager::GetScene(request->name());

	int numObjects = scene->_objects.size();


	for (int i = 0; i < numObjects; i++)
	{
		ObjectReply object;

		Shared<Object> sceneObject = scene->_objects[i];

		Vec3Reply* pos = object.mutable_position();
		pos->set_x(sceneObject->transform.position.x);
		pos->set_y(sceneObject->transform.position.y);
		pos->set_z(sceneObject->transform.position.z);

		QuaternionReply* rot = object.mutable_rotation();
		rot->set_w(sceneObject->transform.rotation.w);
		rot->set_x(sceneObject->transform.rotation.x);
		rot->set_y(sceneObject->transform.rotation.y);
		rot->set_z(sceneObject->transform.rotation.z);

		Vec3Reply* scale = object.mutable_scale();
		scale->set_x(sceneObject->transform.scale.x);
		scale->set_y(sceneObject->transform.scale.y);
		scale->set_z(sceneObject->transform.scale.z);

		MeshRenderer* mesh = sceneObject->GetComponent<MeshRenderer>();
		if (mesh == nullptr) continue;
		if (mesh->ActiveMesh == nullptr) continue;
		List<Vertex> vertexData = mesh->ActiveMesh->_vertex_data;
		for (int j = 0; j < vertexData.size(); j++)
		{
			VertexReply* vertex = object.add_vertices();

			Vec3Reply* vertexPos = vertex->mutable_position();
			vertexPos->set_x(vertexData[j].position.x);
			vertexPos->set_y(vertexData[j].position.y);
			vertexPos->set_z(vertexData[j].position.z);

			Vec3Reply* vertexNorm = vertex->mutable_normal();
			vertexNorm->set_x(vertexData[j].normal.x);
			vertexNorm->set_y(vertexData[j].normal.y);
			vertexNorm->set_z(vertexData[j].normal.z);

			Vec2Reply* vertexUv = vertex->mutable_uv();
			vertexUv->set_x(vertexData[j].uv.x);
			vertexUv->set_y(vertexData[j].uv.y);
		}

		writer->Write(object);
	}

	return grpc::Status::OK;
}

grpc::Status SceneStreamServer::GetSceneList(grpc::ServerContext* context, const Empty* request, SceneListReply* reply)
{
	Logger::Log("Sending scene list");

	for (int i = 0; i < SceneManager::GetScenes().size(); i++)
	{
		reply->add_scenenames(SceneManager::GetScenes()[i]);
	}

	return grpc::Status::OK;
}

grpc::Status SceneStreamServer::AskSceneInfo(grpc::ServerContext* context, const SceneReq* request, SceneInfoReply* reply)
{
	Logger::Log("Sending scene info");

	reply->set_status(true);
	reply->set_totalobjects(SceneManager::GetScene(request->name())->_objects.size());

	return grpc::Status::OK;
}

bool SceneStreamServer::fileExists(std::string fileName)
{
	std::ifstream file(fileName);
	return file.good();
}
