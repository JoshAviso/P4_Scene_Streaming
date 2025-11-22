#include <Objects/ObjectManager.h>

#include <algorithm>

#include <Components/Interfaces/ICollider.h>

void ObjectManager::RemoveObject(Object* obj)
{
	obj->Enabled = false;
	_instance->_objectsToRemove.push_back(obj);
}
Object* ObjectManager::FindObjectByName(const String& name)
{
	for (int i = 0; i < _instance->_objects.size(); i++) {
		if (_instance->_objects[i]->Name == name) {
			return _instance->_objects[i].get();
		}
	}
    return nullptr;
}

void ObjectManager::RegisterCollider(ICollider* collider)
{
	if (collider == nullptr) return;
	_instance->_colliders.push_back(collider);
}
void ObjectManager::UnregisterCollider(ICollider* collider)  
{  
   if (collider == nullptr) return;  

   auto it = std::find(_instance->_colliders.begin(), _instance->_colliders.end(), collider);  
   if (it != _instance->_colliders.end()) {  
       _instance->_colliders.erase(it);  
   }  
}

void ObjectManager::Update(float deltaTime)
{
	List<Object*> objects;
	for (int i = 0; i < _objects.size(); i++)
		objects.push_back(_objects[i].get());

	for (int i = 0; i < objects.size(); i++) {

		if (!objects[i]->Enabled) continue;
		List<IUpdateComponent*> updateComponents = objects[i]->GetComponents<IUpdateComponent>();
		for (int j = 0; j < updateComponents.size(); j++)
		{
			if(updateComponents[j] != nullptr) updateComponents[j]->Update(deltaTime);
		}
	}

	CheckCollisions();

	for (int i = 0; i < _objectsToRemove.size(); i++) {
		RemoveObjConcrete(_objectsToRemove[i]);
	}
	_objectsToRemove.clear();
}

void ObjectManager::CheckCollisions()
{
	for (int i = 0; i < _colliders.size(); i++) {
		ICollider* a = _colliders[i];
		for (int j = i + 1; j < _colliders.size(); j++) {
			ICollider* b = _colliders[j];
			if (a == b) continue;

			if (b->_owner->Intersects(a->_owner->Position) || a->_owner->Intersects(b->_owner->Position)) {
				if (b->_collisions[a] || a->_collisions[b]) {
					a->OnCollisionContinue(b);
					b->OnCollisionContinue(a);
				}
				else {
					a->OnCollisionEnter(b);
					b->OnCollisionEnter(a);
				}

				b->_collisions[a] = true;
				a->_collisions[b] = true;
			}
			else {
				if (b->_collisions[a] || b->_collisions[a]) {
					a->OnCollisionExit(b);
					b->OnCollisionExit(a);
				}

				b->_collisions[a] = false;
				a->_collisions[b] = false;
			}
		}
	}
}

void ObjectManager::RenderObjectsTo(sf::RenderWindow& window)
{
	List<IRenderer*> renderers{};
	for (int i = 0; i < _objects.size(); i++)
	{
		if (!_objects[i]->Enabled) continue;
		List<IRenderer*> renderComponents = _objects[i]->GetComponents<IRenderer>();
		for (int j = 0; j < renderComponents.size(); j++)
		{
			renderers.push_back(renderComponents[j]);
		}
	}

	std::stable_sort(renderers.begin(), renderers.end(), 
		[](IRenderer* a, IRenderer* b) {
			return a->RenderLayer < b->RenderLayer;
		});

	for (int i = 0; i < renderers.size(); i++) {
		renderers[i]->Render(window);
	}
}

void ObjectManager::RemoveObjConcrete(Object* obj)
{
	auto it = std::find_if(_instance->_objects.begin(), _instance->_objects.end(),
		[obj](const Unique<Object>& ptr) {
			return ptr.get() == obj;
		});
	if (it != _instance->_objects.end()) {
		_instance->_objects.erase(it);
	}
}

// SINGLETON 
ObjectManager* ObjectManager::_instance = nullptr;
ObjectManager* ObjectManager::Initialize()
{
    if (_instance != nullptr) return _instance;

    _instance = new ObjectManager();
    return _instance;
}

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager() {}
