#pragma once

#include <Common.hpp>
#include <Scenes/SceneManager.h>

class Window;

class Application {
public:
	struct Desc {
		Window* window;
		SceneManager::Desc sceneManagerDesc;
	};

	static void Quit();
	static void Run();

	static Window* GetWindow();

private:
	static bool _shouldClose;
	Unique<Window> _window = nullptr;

	static void ProcessInput();
	static void Update(float deltaTime);
	static void Render();

	// SINGLETON
public:
	static Application* Initialize(const Desc desc);
private:
	static Application* _instance;
	explicit Application(const Desc& desc);
	~Application() = default;
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
};