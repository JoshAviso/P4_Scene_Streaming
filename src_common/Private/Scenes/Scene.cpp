#include <Scenes/Scene.h>

#include <objects/ObjectManager.h>

Scene::Scene(String name) : SceneName(name){}

void Scene::Open()
{
	for (int i = 0; i < _objects.size(); i++) {
		ObjectManager::RegisterObject(_objects[i]);
	}
}

Shared<Object> Scene::AddObject(Object* obj)
{
	Shared<Object> o = Shared<Object>(obj);
	_objects.push_back(o);
	return o;
}
