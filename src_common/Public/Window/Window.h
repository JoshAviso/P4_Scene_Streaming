#pragma once

#include <GLFW/glfw3.h>

#include <Common.hpp>
#include <Input/Input.h>
#include <Color.hpp>
#include <String.hpp>
#include <mutex>

class Window {
public:
	struct Desc {
		String name;
		int width;
		int height;
		Color clearColor;
	};

public:
	std::mutex glMtx;
private:
	GLFWwindow* _window = nullptr;
	GLFWwindow* _resourceWindow = nullptr;
	Desc _desc;

public:
	explicit Window(const Desc desc);
	~Window();
	bool Initialize();
	bool IsInitialized() const;
	bool ShouldClose() const;
	float AspectRatio() const;

	friend class GraphicsSystem;
	friend class ThreadGLContext;
	friend class Input;
};