#pragma once

#include <Resources/IResource.h>
#include <glad/glad.h>
#include <Math/Vertex.h>
#include <Common.hpp>

class Mesh : public IResource {
private:
	GLuint _vao, _vbo;
	List<Vertex> _vertex_data;
	List<GLfloat> _gl_vert_data;
	bool _loaded = false;

public:
	~Mesh();
	bool LoadFromVertexData(const List<Vertex>& vertData);

private:
	bool LoadFromFile(String filepath) override;
	void LogGLErrorsIfExists(String prefix);
	bool LoadFromVertexData();

	friend class MeshRenderer;
	friend class ResourceManager;
	friend class GraphicsSystem;
};