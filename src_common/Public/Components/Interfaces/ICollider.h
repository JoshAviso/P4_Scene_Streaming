#pragma once

#include <Components/IComponent.h>
#include <Common.hpp>
#include <Objects/ObjectManager.h>

class ICollider : virtual public IComponent {
protected:
	virtual void OnCollisionEnter(ICollider* other){};
	virtual void OnCollisionContinue(ICollider* other) {};
	virtual void OnCollisionExit(ICollider* other) {};

public:
	ICollider() { ObjectManager::RegisterCollider(this); }
	virtual ~ICollider() { ObjectManager::UnregisterCollider(this); }

private:
	Dict<ICollider*, bool> _collisions;
	friend class ObjectManager;
};