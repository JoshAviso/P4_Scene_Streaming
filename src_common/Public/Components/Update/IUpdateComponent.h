#pragma once

#include <Components/IComponent.h>
#include <Objects/ObjectManager.h>

class IUpdateComponent : virtual public IComponent
{
public:
	virtual ~IUpdateComponent() = default;
protected:
	virtual void Update(float deltaTime){};
	friend class ObjectManager;
};

