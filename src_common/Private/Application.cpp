#include <Application.h>

#include <Logger.hpp>
#include <chrono>  
#include <stdexcept>
#include <GLFW/glfw3.h>

#include <Graphics/GraphicsSystem.h>
#include <Window/Window.h>
#include <Resources/ResourceManager.h>
#include <Input/Input.h>
#include <Objects/ObjectManager.h>
#include <Camera/CameraManager.h>
#include <Scenes/SceneManager.h>

Application* Application::_instance = nullptr;
Application* Application::Initialize(const Desc desc)
{
	if (_instance != nullptr) 
		Logger::LogWarning("Trying to re-initialize Application. Application already initialized!");
	else _instance = new Application(desc);

    if (desc.window == nullptr) throw std::runtime_error("Trying to initialize Application with null window.");
    _instance->_window = Unique<Window>(desc.window);

    GraphicsSystem::Initialize();

    CameraManager::Initialize();
    ResourceManager::Initialize();
    Input::Initialize();
    ObjectManager::Initialize();
    SceneManager::Initialize(desc.sceneManagerDesc);

	return _instance;
}

Application::Application(const Desc& desc)
{

}

bool Application::_shouldClose = false;
void Application::Quit()
{
	if (!_instance) {
		Logger::LogWarning("Trying to quit the application before it initialized!"); return;
	}
	_shouldClose = true;
}

void Application::Run()
{  
    if (!_instance) {
        Logger::LogWarning("Trying to run the application before it initialized!");
        return;
    }

    auto lastTime = std::chrono::high_resolution_clock::now();  
    while (!_shouldClose) {  
        Input::ResetInput();
        ProcessInput();  

        auto currentTime = std::chrono::high_resolution_clock::now();  
        std::chrono::duration<float> deltaTime = currentTime - lastTime;  
        lastTime = currentTime;  

        Update(deltaTime.count());  
        Render();  
    }  
}

Window* Application::GetWindow()
{
    if (_instance == nullptr) return nullptr;
    if (_instance->_window == nullptr) return nullptr;
    return _instance->_window.get();
}

void Application::ProcessInput()
{
    glfwPollEvents();
    if (_instance->_window->ShouldClose())
        _shouldClose = true;
}

void Application::Update(float deltaTime)
{
    ObjectManager::_instance->Update(deltaTime);
}

void Application::Render()
{
    GraphicsSystem::Render(_instance->_window.get());
}
