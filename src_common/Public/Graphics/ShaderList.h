#pragma once

#include <Common.hpp>
#include <String.hpp>
#include <Graphics/Shader.h>
#include <Resources/IShaderResource.h>

#include <stdexcept>
#include <mutex>

class ShaderList
{
private:
	Dict<String, Shared<Shader>> _shaders;
	std::mutex _resourceGuard;

public:
	static Shared<Shader> GenerateShader(String name, Shared<VertexShader> vertex_shader, Shared<FragShader> frag_shader);
	static Shared<Shader> GetShader(String name);

	// SINGLETON
private:
	static ShaderList* _instance;
	explicit ShaderList() = default;
	~ShaderList() = default;
	ShaderList(const ShaderList&) = delete;
	ShaderList& operator=(const ShaderList&) = delete;

};

