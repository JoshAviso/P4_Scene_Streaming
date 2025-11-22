#pragma once

#include <Components/IComponent.h>
#include <Color.hpp>

class IRenderer : virtual public IComponent
{
public:
	Color base_color;
	int RenderLayer = 0;
	virtual ~IRenderer() = default;

protected:
	virtual void Render() = 0;
	friend class ObjectManager;
};

