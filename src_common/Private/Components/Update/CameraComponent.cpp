#include <Components/Update/CameraComponent.h>

#include <Camera/CameraManager.h>

CameraComponent::CameraComponent(String CameraName, Camera::Projection* projection, bool replaceActive) : _name(CameraName), projection(projection)
{
	_camera = CameraManager::RegisterCamera(CameraName, new Camera(), replaceActive);
}

CameraComponent::~CameraComponent()
{
	CameraManager::UnregisterCamera(_name);
}

void CameraComponent::Update(float deltaTime)
{
	_camera->projection = projection;
	_camera->position = _owner->transform.position;
	_camera->rotation = _owner->transform.rotation;
}
