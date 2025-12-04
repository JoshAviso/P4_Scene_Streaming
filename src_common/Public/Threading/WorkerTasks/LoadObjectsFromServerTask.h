#pragma once

#include <Threading/WorkerTasks/IWorkerTask.h>

#include <Common.hpp>

#include <Math/Vec3.h>
#include <Math/Quaternion.h>
#include <Math/Vertex.h>

#include <Scenes/SceneManager.h>

#include <Resources/Mesh.h>
#include <Components/Renderers/MeshRenderer.h>

#include <Threading/ThreadPoolManager.h>
#include <Threading/WorkerTasks/MeshVertexLoadTask.h>

#include <Logger.hpp>

#include <Threading/IThreadFinishedCallback.h>

#include <SceneStreaming.grpc.pb.h>
#include <Networking/SceneStreamClient.h>

class LoadObjectsFromServerTask : public IWorkerTask, public IThreadFinishedCallback {
private:
	String _sceneName;
	Shared<Scene> _scene = nullptr;
	int _pendingChildren = 0;
	int _finishedChildren = 0;

public:
	LoadObjectsFromServerTask(const String sceneName) : _sceneName(sceneName) {
		_scene = SceneManager::GetScene(_sceneName);
	};

	void DoWorkerTask(int id) override {

		// Request the server to get scene meta info
		bool exists;
		int objCount;

		SceneStreamClient::Instance()->AskSceneInfo(&exists, &objCount, _sceneName);

		if (!exists) {
			Logger::LogWarning("Tried to load a scene that does not exist on the server");
			return;
		}

		if (_scene == nullptr)
			_scene = SceneManager::AddScene(new Scene(_sceneName));

		_scene->SceneCompletion = 0;
		_scene->ScenePending = objCount;
		_scene->IsLoading = true;

		_scene->_objects.clear();

		SceneReq request;
		request.set_name(_sceneName);

		grpc::ClientContext context;
		// Make call to client for server
		std::shared_ptr<grpc::ClientReader<ObjectReply>> reader =
			SceneStreamClient::Instance()->_stub->GetSceneObjectData(&context, request);

		// Read and parse object info in reply
		ObjectReply obj;
		while (reader->Read(&obj))
		{
			// Process object data into the following variables
			Vec3 objPosition = Vec3(obj.position().x(), obj.position().y(), obj.position().z());
			Quaternion objRotation = Quaternion(
				obj.rotation().w(), obj.rotation().x(), obj.rotation().y(), obj.rotation().z());
			Vec3 objScale = Vec3(obj.scale().x(), obj.scale().y(), obj.scale().z());

			List<Vertex> vertex_data;

			Shared<Object> obj = _scene->AddObject(new Object("Object" + std::to_string(_pendingChildren + 1)));
			obj->transform.position = objPosition;
			obj->transform.scale = objScale;
			obj->transform.rotation = objRotation;

			// Only make a mesh if it does indeed have one
			if (vertex_data.size() > 0) {
				Shared<Mesh> mesh = Make_Shared<Mesh>();
				MeshRenderer* renderer = obj->AddComponent(new MeshRenderer());
				renderer->ActiveMesh = mesh;

				// Create a thread for mesh to begin loading data without 
				// blocking this thread or waiting for all data to be sent
				ThreadPoolManager::GetThreadPool("Main")
					->ScheduleTask(new MeshVertexLoadTask(mesh, vertex_data, this));
				_pendingChildren++;
			}

		}

		grpc::Status status = reader->Finish();
		if (status.ok()) Logger::Log("Successful object stream for Scene: " + _sceneName);
		else {
			Logger::LogWarning("Object streaming failed for Scene: " + _sceneName);
			return;
		}
		
		// Dont finish the thread while children are still pending
		while (_finishedChildren < _pendingChildren);
	}

	void OnThreadFinished(int id) override {
		_finishedChildren++;
		if (!_scene) _scene->SceneCompletion = _scene->SceneCompletion + 1;
	}
};