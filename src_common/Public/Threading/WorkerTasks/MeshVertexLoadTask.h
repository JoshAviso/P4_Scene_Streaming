#pragma once

#include <Threading/WorkerTasks/IWorkerTask.h>
#include <Resources/Mesh.h>
#include <Threading/IThreadFinishedCallback.h>

class MeshVertexLoadTask : public IWorkerTask {
private:
	Shared<Mesh> _meshToLoad;
	List<Vertex> _vert_data;
	IThreadFinishedCallback* _callback = nullptr;

public:
	MeshVertexLoadTask(
		const Shared<Mesh> meshToLoad, const List<Vertex>& vert_data, 
		IThreadFinishedCallback* callback) : 
		_meshToLoad(meshToLoad), _vert_data(vert_data), _callback(callback) {} ;
	
	void DoWorkerTask(int id) override {
		if (_meshToLoad == nullptr) return;
		_meshToLoad->LoadFromVertexData(_vert_data);

		if (_callback)
			_callback->OnThreadFinished(id);
	}
};