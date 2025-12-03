#pragma once

#include <Threading/WorkerTasks/IWorkerTask.h>
#include <Resources/IResource.h>
#include <Resources/ResourceManager.h>
#include <Threading/IThread.h>
#include <Threading/IThreadFinishedCallback.h>
#include <Threading/ThreadGLContext.h>
#include <Application.h>

template <typename TResource>
class LoadResourceTask : public IWorkerTask {
private:
	String _resourceName;
	String _resourcePath;
	uint _delay;
	IThreadFinishedCallback* _callback;

public:
	LoadResourceTask(String resourceName, String path, uint delay = 0, IThreadFinishedCallback* callback = nullptr) : 
		_resourceName(resourceName), _resourcePath(path), _delay(delay), _callback(callback) {
		static_assert(std::is_base_of<IResource, TResource>::value, "TResource must inherit from IResource");
	};

	void DoWorkerTask(int id) override {
		IThread::Sleep(_delay);
		/* Actual Threaded resource loading
		Shared<ThreadGLContext> ctx = Make_Shared<ThreadGLContext>(Application::GetWindow());
		ctx->BeginUse();
		ResourceManager::LoadFromFile<TResource>(_resourceName, _resourcePath);
		ctx->FinishUse();
		*/
		if (_callback != nullptr)
			_callback->OnThreadFinished(id);
	}

};