#pragma once

#include <Common.hpp>
#include <String.hpp>

class Window;
class Mesh;

class GraphicsSystem {
public:
	static void Render(Window* window);
	static void SubmitMeshForUpload(Mesh* mesh);
	static void UploadSubmittedMeshes();

private:
	List<Mesh*> _meshesToUpload;
	static void LogGLErrorsIfExists(const String text);

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