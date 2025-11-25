#pragma once

#include <Common.hpp>
#include <String.hpp>

class Camera;

class CameraManager {
public:
	static Shared<Camera> GetActiveCam();
	static Shared<Camera> SetActiveCam(String name);

private:
	Dict<String, Shared<Camera>> _cameras;
	String _activeCam = String();
	static Shared<Camera> RegisterCamera(String name, Camera* camera, bool makeActive);
	static void UnregisterCamera(String name);

// SINGLETON
public:
	static CameraManager* Initialize();
private:
	static CameraManager* _instance;
	explicit CameraManager();
	~CameraManager();
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

	friend class CameraComponent;
};