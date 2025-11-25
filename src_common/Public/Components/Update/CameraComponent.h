#pragma once

#include <Components/Update/IUpdateComponent.h>
#include <Camera/Camera.h>

class CameraComponent : public IUpdateComponent {
public:
	Camera::Projection* projection = nullptr;

	CameraComponent(String CameraName, Camera::Projection* projection, bool replaceActive = false);
	~CameraComponent();

private:
	Shared<Camera> _camera = nullptr;
	String _name;

private:
	void Update(float deltaTime) override ;
	friend class ObjectManager;
};
