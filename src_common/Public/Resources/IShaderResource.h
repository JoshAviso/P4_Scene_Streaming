#pragma once
#include <Resources/IResource.h>
#include <glad/glad.h>
class IShaderResource : public IResource {
protected:
	GLuint shader_data;
	virtual bool LoadFromFile(String filepath) = 0;
	friend class Shader;
};

class VertexShader : public IShaderResource {
private:
	bool LoadFromFile(String filepath);
	friend class ResourceManager;
};

class FragShader : public IShaderResource {
private:
	bool LoadFromFile(String filepath);
	friend class ResourceManager;
};