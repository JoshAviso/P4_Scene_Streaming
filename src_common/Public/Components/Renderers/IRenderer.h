#pragma once

#include <Components/IComponent.h>
#include <Color.hpp>

class Camera;
class IRenderer : virtual public IComponent
{
public:
	Color base_color;
	int RenderLayer = 0;
	virtual ~IRenderer() = default;

protected:
	virtual void Render(Camera* camera) = 0;
	friend class ObjectManager;
};

