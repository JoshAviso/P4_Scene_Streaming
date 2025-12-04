#pragma once

#include <Resources/IResource.h>
#include <glad/glad.h>
#include <Math/Vertex.h>
#include <Common.hpp>

class Mesh : public IResource {
private:
	GLuint _vao, _vbo;
	List<Vertex> _vertex_data;
	bool _loaded = false;

public:
	bool LoadFromVertexData(const List<Vertex>& vertData);
	~Mesh();

private:
	bool LoadFromFile(String filepath) override;
	void LogGLErrorsIfExists(String prefix);
	bool LoadFromVertexData();

	friend class MeshRenderer;
	friend class ResourceManager;
};