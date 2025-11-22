#pragma once

class Object;

class IComponent
{
public:
	Object* GetOwner() { return _owner; };

protected:
	Object* _owner = nullptr;

public:
	virtual ~IComponent() = default;

	friend class Object;
};

