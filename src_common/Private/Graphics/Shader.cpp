#include <Graphics/Shader.h>

#include <Logger.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() {
	_shader_program = glCreateProgram();
}

void Shader::PassData(String name, const Mat4 mat4)
{
	if (!_linked) {
		Logger::LogWarning("Trying to pass data to unlinked shader!");
		return;
	}

	glm::mat4 tf = mat4;
	glUseProgram(_shader_program);
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(tf));
	glUseProgram(0);
}

void Shader::PassData(String name, const Transform transform)
{
	if (!_linked) {
		Logger::LogWarning("Trying to pass data to unlinked shader!");
		return;
	}
	
	glm::mat4 tf = Mat4(transform);
	glUseProgram(_shader_program);
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(tf));
	glUseProgram(0);

}

void Shader::PassData(String name, const Vec3 vector)
{
	if (!_linked) {
		Logger::LogWarning("Trying to pass data to unlinked shader!");
		return;
	}

	glm::vec3 vec = vector;
	glUseProgram(_shader_program);
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec));
	glUseProgram(0);
}

uint Shader::GetUniformLocation(String name)
{
	if (!_linked) return uint();
	uint res = glGetUniformLocation(_shader_program, name.c_str());
	return res;
}

void Shader::Bind(Shared<VertexShader> vert_shader)
{
	if (vert_shader == nullptr) return;
	_vertex_shader = vert_shader;
	glAttachShader(_shader_program, _vertex_shader->shader_data);
	if (_vertex_shader && _frag_shader) {
		glLinkProgram(_shader_program);
		_linked = true;
	}
}

void Shader::Bind(Shared<FragShader> frag_shader)
{
	if (frag_shader == nullptr) return;
	_frag_shader = frag_shader;
	glAttachShader(_shader_program, _frag_shader->shader_data);
	if (_vertex_shader && _frag_shader) {
		glLinkProgram(_shader_program);
		_linked = true;
	}
}

void Shader::Use()
{
	if (!_linked) {
		Logger::LogWarning("Trying to use unlinked shader!");
		return;
	}
	glUseProgram(_shader_program);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}