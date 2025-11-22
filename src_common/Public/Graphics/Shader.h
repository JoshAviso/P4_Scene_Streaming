#pragma once

#include <Resources/IShaderResource.h>
#include <Common.hpp>
#include <glad/glad.h>

#include <Math/Transform.h>
#include <Math/Vec3.h>

class Shader {
private:
	GLuint _shader_program;
	Shared<VertexShader> _vertex_shader = nullptr;
	Shared<FragShader> _frag_shader = nullptr;
	bool _linked = false;

public:
	Shader();

	void PassData(String name, const Transform transform);
	void PassData(String name, const Vec3 vector);

private:
	uint GetUniformLocation(String name);

private:
	void Bind(Shared<VertexShader> vert_shader);
	void Bind(Shared<FragShader> frag_shader);

	void Use();

	friend class MeshRenderer;
	friend class ShaderList;
};