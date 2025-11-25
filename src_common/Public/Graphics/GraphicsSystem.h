#pragma once

#include <Common.hpp>

class Window;

class GraphicsSystem {
public:
	static void Render(Window* window);

// SINGLETON
public:
	static GraphicsSystem* Initialize();
private:
	static GraphicsSystem* _instance;
	explicit GraphicsSystem();
	~GraphicsSystem();
	GraphicsSystem(const GraphicsSystem&) = delete;
	GraphicsSystem& operator=(const GraphicsSystem&) = delete;

};