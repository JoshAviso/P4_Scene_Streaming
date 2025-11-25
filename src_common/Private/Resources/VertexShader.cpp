#include <Resources/IShaderResource.h>

#include <fstream>
#include <sstream>
#include <String.hpp>

VertexShader::~VertexShader()
{
    glDeleteShader(shader_data);
}

bool VertexShader::LoadFromFile(String filepath)
{
    /* Read the Shaders from their source files */
    std::fstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    String str = buffer.str();
    const char* c = str.c_str();
    shader_data = glCreateShader(GL_VERTEX_SHADER);

    /* Compile Shaders */
    glShaderSource(shader_data, 1, &c, NULL);
    glCompileShader(shader_data);

    return true;
}