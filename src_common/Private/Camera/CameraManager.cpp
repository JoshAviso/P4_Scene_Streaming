#include <Camera/CameraManager.h>

#include <Camera/Camera.h>

Shared<Camera> CameraManager::GetActiveCam()
{
    if (_instance->_cameras.find(_instance->_activeCam) == _instance->_cameras.end()) return nullptr;
    return _instance->_cameras[_instance->_activeCam];
}

Shared<Camera> CameraManager::SetActiveCam(String name)
{
    if (_instance->_cameras.find(name) == _instance->_cameras.end()) return nullptr;
    _instance->_activeCam = name;
    return GetActiveCam();
}

Shared<Camera> CameraManager::RegisterCamera(String name, Camera* camera, bool makeActive)
{
    if (_instance->_cameras.find(name) != _instance->_cameras.end()) 
        return _instance->_cameras[name];

    Shared<Camera> cam = Shared<Camera>(camera);
    _instance->_cameras[name] = cam;
    if (makeActive || _instance->_cameras.size() <= 1) 
        _instance->_activeCam = name;
    return cam;
}

void CameraManager::UnregisterCamera(String name)
{
    auto it = _instance->_cameras.find(name);
    if (it != _instance->_cameras.end()) {
        _instance->_cameras.erase(it);
    }
}

// SINGLETON 
CameraManager* CameraManager::_instance = nullptr;
CameraManager* CameraManager::Initialize()
{
    if (_instance != nullptr) return _instance;

    _instance = new CameraManager();
    return _instance;
}

CameraManager::CameraManager()
{

}

CameraManager::~CameraManager() {}
