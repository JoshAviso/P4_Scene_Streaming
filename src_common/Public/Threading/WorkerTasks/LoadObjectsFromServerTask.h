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

class LoadObjectsFromServerTask : public IWorkerTask, public IThreadFinishedCallback {
private:
	String _sceneName;
	Shared<Scene> _scene = nullptr;
	int _objCount = 0;
	int _pendingChildren = 0;
	int _finishedChildren = 0;

public:
	LoadObjectsFromServer(const String sceneName, int objCount) : _sceneName(sceneName), _objCount(objCount) {
		_scene = SceneManager::GetScene(_sceneName);
	};

	void DoWorkerTask(int id) override {
		if (_scene == nullptr)
			_scene = SceneManager::AddScene(new Scene(_sceneName));
		
		_scene->SceneCompletion = 0;
		_scene->ScenePending = _objCount;
		_scene->IsLoading = true;

		// Make call to client for server
		// Somehow get object info and do the following for each

		{
			// Process object data into the following variables
			Vec3 objPosition;
			Quaternion objRotation;
			Vec3 objScale;

			List<Vertex> vertex_data;

			Shared<Object> obj = scene->AddObject(new Object("ObjectX")); // TODO : Modify to name based on pang ilang object sya to prevent dupe names
			obj->transform.position = objPosition;
			obj->transform.scale = objScale;
			obj->transform.rotation = objRotation;

			// Only make a mesh if it does indeed have one
			if (vertex_data.size() > 0) {
				Shared<Mesh> mesh = Make_Shared<Mesh>();
				Shared<MeshRenderer> renderer = obj->AddComponent<MeshRenderer>();
				renderer->ActiveMesh = mesh;

				// Create a thread for mesh to begin loading data without blocking this thread
				ThreadPoolManager::GetThreadPool("Main")->ScheduleTask(new MeshVertexLoadTask(mesh, vertex_data, this));
				_pendingChildren++;
			}

			while (_finishedChildren < _pendingChildren);
		}

		void OnThreadFinished(int id) override {
			_finishedChildren++;
			if (!_scene) _scene->SceneCompletion = _scene->SceneCompletion + 1;
		}
	}
};