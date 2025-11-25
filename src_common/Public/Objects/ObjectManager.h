#pragma once

#include <Common.hpp>
#include <Objects/Object.h>
#include <Logger.hpp>
#include <String.hpp>

#include <Components/Update/IUpdateComponent.h>
#include <Components/Renderers/IRenderer.h>

class ICollider;
class Window;
class Camera;

class ObjectManager
{
public:
	template <typename TObject>
	static TObject* RegisterObject(TObject* obj){
		static_assert(std::is_base_of<Object, TObject>::value, "Pointer must be an object.");
		_instance->_objects.push_back(Unique<Object>(obj));

		return obj;
	}

	static void RemoveObject(Object* obj);
	static Object* FindObjectByName(const String& name);

	static void RegisterCollider(ICollider* collider);
	static void UnregisterCollider(ICollider* collider);

private:
	List<ICollider*> _colliders;
	List<Unique<Object>> _objects;
	List<Object*> _objectsToRemove;

	void Update(float deltaTime);
	//void CheckCollisions();
	void RenderObjectsTo(Camera* camera);
	void RenderObjects(Camera* camera);
	void RemoveObjConcrete(Object* obj);

// SINGLETON
public:
	static ObjectManager* Initialize();
private:
	static ObjectManager* _instance;
	explicit ObjectManager();
	~ObjectManager();
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator=(const ObjectManager&) = delete;

public:
	friend class GraphicsSystem;
	friend class Application;
};

