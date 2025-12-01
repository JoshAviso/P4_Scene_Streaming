#pragma once

#include <Common.hpp>
#include <Objects/Object.h>

class Scene {
public:
	Scene(String name);
	String GetName() { return SceneName;  };

private:
	String SceneName = "";
	List<Shared<Object>> _objects;
	
private:
	void Open();
	Shared<Object> AddObject(Object* obj);

	friend class SceneManager;
};