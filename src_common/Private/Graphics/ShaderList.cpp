#include <Graphics/ShaderList.h>

#include <Logger.hpp>

ShaderList* ShaderList::_instance = nullptr
Shared<Shader> ShaderList::GenerateShader(String name, Shared<VertexShader> vertex_shader, Shared<FragShader> frag_shader)
{
	if (_instance == nullptr) _instance = new ShaderList();

	_instance->_resourceGuard.lock();
	if (_instance->_shaders.find(name) != _instance->_shaders.end()) {
		Logger::LogWarning("Trying to create existing shader: " + name);
		return _instance->_shaders[name];
	}
	_instance->_resourceGuard.unlock();

	if (!vertex_shader && !frag_shader) {
		Logger::LogWarning("Failed to create shader: " + name);
		return nullptr;
	}

	Shared<Shader> shader = Make_Shared<Shader>();
	shader->Bind(vertex_shader);
	shader->Bind(frag_shader);

	_instance->_resourceGuard.lock();
	_instance->_shaders[name] = shader;
	_instance->_resourceGuard.unlock();

	return _instance->_shaders[name];
}

Shared<Shader> ShaderList::GetShader(String name)
{
    if (_instance == nullptr) return nullptr;

	_instance->_resourceGuard.lock();
	if (_instance->_shaders.find(name) == _instance->_shaders.end()) {
		Logger::LogWarning("Cannot find shader: " + name);
	}
	_instance->_resourceGuard.unlock();
	
	return _instance->_shaders[name];
}
