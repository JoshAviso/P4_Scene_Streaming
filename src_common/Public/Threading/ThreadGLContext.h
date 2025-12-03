#pragma once

#include <Window/Window.h>
#include <GLFW/glfw3.h>
#include <Logger.hpp>

class ThreadGLContext {
private:
	Window* _mainWindow = nullptr;
	GLFWwindow* _threadWindow = nullptr;

public:
	ThreadGLContext(Window* mainWindow) : _mainWindow(mainWindow){
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		_threadWindow = glfwCreateWindow(1, 1, "", nullptr, mainWindow->_resourceWindow);
		if (!_threadWindow)
			Logger::LogError("Failed to create thread context loader window");
		glfwMakeContextCurrent(_threadWindow);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			Logger::LogError("Failed to initialize thread glad loader");
	};
	void BeginUse() {
		_mainWindow->glMtx.lock();
	}
	void FinishUse() {
		glFlush();
		glfwMakeContextCurrent(nullptr);
		_mainWindow->glMtx.unlock();
	}
	~ThreadGLContext() {
		glfwDestroyWindow(_threadWindow);
	}

};