#pragma once

#include <Window/Window.h>
#include <GLFW/glfw3.h>
#include <Logger.hpp>

class ThreadGLContext {
private:
	GLFWwindow* _threadWindow = nullptr;

public:
	ThreadGLContext(Window* mainWindow) {
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		_threadWindow = glfwCreateWindow(1, 1, "", nullptr, mainWindow->_window);
		if (!_threadWindow)
			Logger::LogError("Failed to create thread context loader window");

	};
	void BeginUse() {
		glfwMakeContextCurrent(_threadWindow);
	}
	void FinishUse() {
		glfwMakeContextCurrent(nullptr);
	}
	~ThreadGLContext() {
		glfwDestroyWindow(_threadWindow);
	}

};