#include <Window/Window.h>

#include <GLFW/glfw3.h>
#include <Logger.hpp>

Window::Window(const Desc desc) : _desc(desc){}

Window::~Window()
{
	glfwDestroyWindow(_window);
}

bool Window::Initialize()
{
	_window = glfwCreateWindow(_desc.width, _desc.height, _desc.name.c_str(), NULL, NULL);
	if (!_window) {
		_window = nullptr;
		Logger::LogError("Failed to create main window!");
		return false;
	}
	glfwMakeContextCurrent(_window);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	_resourceWindow = glfwCreateWindow(1, 1, "", nullptr, _window);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	if (!_resourceWindow)
		Logger::LogError("Failed to create resource window");

	glfwSetKeyCallback(_window, Input::KeyCallback);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetCursorPosCallback(_window, Input::CursorCallback);
	glfwSetMouseButtonCallback(_window, Input::MouseButtonCallback);
	glfwSetScrollCallback(_window, Input::ScrollCallback);

	return true;
}

bool Window::IsInitialized() const { return _window != nullptr; }

bool Window::ShouldClose() const { return _window == nullptr || glfwWindowShouldClose(_window); }

float Window::AspectRatio() const
{
	return (float)_desc.width / (float)_desc.height;
}
