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

        // CHANGE: now reading ObjectBatchReply instead of ObjectReply
        std::shared_ptr<grpc::ClientReader<ObjectBatchReply>> reader =
            SceneStreamClient::Instance()->_stub->GetSceneObjectData(&context, request);

        ObjectBatchReply batch;

        while (reader->Read(&batch))
        {
            // Iterate over objects inside the batch
            for (const auto& objMsg : batch.objects())
            {
                // --- Parse transform ---
                Vec3 objPosition(
                    objMsg.position().x(),
                    objMsg.position().y(),
                    objMsg.position().z()
                );

                Quaternion objRotation(
                    objMsg.rotation().w(),
                    objMsg.rotation().x(),
                    objMsg.rotation().y(),
                    objMsg.rotation().z()
                );

                Vec3 objScale(
                    objMsg.scale().x(),
                    objMsg.scale().y(),
                    objMsg.scale().z()
                );

                // --- Create the scene object ---
                Shared<Object> obj =
                    _scene->AddObject(new Object("Object" + std::to_string(_pendingChildren + 1)));

                obj->transform.position = objPosition;
                obj->transform.rotation = objRotation;
                obj->transform.scale = objScale;

                // --- Parse vertex data ---
                List<Vertex> vertex_data;
                vertex_data.reserve(objMsg.vertices_size());

                for (const auto& v : objMsg.vertices())
                {
                    Vertex vert;
                    vert.position = Vec3(v.position().x(), v.position().y(), v.position().z());
                    vert.normal = Vec3(v.normal().x(), v.normal().y(), v.normal().z());
                    vert.uv = Vec2(v.uv().x(), v.uv().y());

                    vertex_data.push_back(vert);
                }

                // --- Only create mesh if it actually has vertices ---
                if (!vertex_data.empty()) {

                    Shared<Mesh> mesh = Make_Shared<Mesh>();
                    MeshRenderer* renderer = obj->AddComponent(new MeshRenderer());
                    renderer->ActiveMesh = mesh;

                    ThreadPoolManager::GetThreadPool("Main")
                        ->ScheduleTask(new MeshVertexLoadTask(mesh, vertex_data, this));

                    _pendingChildren++;
                }
            }
        }

        grpc::Status status = reader->Finish();
        if (status.ok()) {
            Logger::Log("Successful object stream for Scene: " + _sceneName);
        }
        else {
            Logger::LogWarning("Object streaming failed for Scene: " + _sceneName);
            return;
        }

        // Wait for all mesh loading tasks
        while (_finishedChildren < _pendingChildren);
    }


	void OnThreadFinished(int id) override {
		_finishedChildren++;
		if (!_scene) _scene->SceneCompletion = _scene->SceneCompletion + 1;
	}
};