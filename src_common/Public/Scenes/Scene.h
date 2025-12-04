#pragma once

#include <Common.hpp>
#include <Objects/Object.h>
#include <mutex>

class Scene {
public:
	Scene(String name);
	String GetName() { return SceneName;  };
	List<Shared<Object>> _objects;
	bool IsLoading = false;
	bool IsFullyLoaded() const { return SceneProgress() >= 1.f; };
	float SceneProgress() const { 
		if (ScenePending == 0) return 1.f;
		return (float)SceneCompletion / (float)ScenePending;
	}
	int SceneCompletion = 0;
	std::mutex sceneMtx;
	Shared<Object> AddObject(Object* obj);

private:
	String SceneName = "";
	int ScenePending = 0;
	
private:
	void Open();

	friend class SceneManager;
	friend class LoadObjectsFromServerTask;
};