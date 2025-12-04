#include <Resources/Mesh.h>

#include <tiny_obj_loader.h>

#include <Common.hpp>
#include <Logger.hpp>

#include <Graphics/GraphicsSystem.h>

Mesh::~Mesh()
{
	if (_loaded) {
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
	}
}

bool Mesh::LoadFromVertexData(const List<Vertex>& vertData)
{
	if (vertData.size() <= 0) {
		Logger::LogWarning("Tried to load mesh from empty vertex data");
		return false;
	}
	_vertex_data = vertData;
	_loaded = false;
	return LoadFromVertexData();
}

bool Mesh::LoadFromFile(String filepath)
{
	tinyobj::attrib_t attribs;
	List<tinyobj::shape_t> shapes;
	List<tinyobj::material_t> mats;
	String warn, err;

	bool success = tinyobj::LoadObj(&attribs, &shapes, &mats, &warn, &err, filepath.c_str());
	if (!success) {
		Logger::LogWarning("Failed to load model: " + filepath);
		return false;
	}

	for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
		tinyobj::index_t vData = shapes[0].mesh.indices[i];

		/* Load X, Y, Z, Nx, Ny, Nz, U, and V data */
		float x = attribs.vertices[(vData.vertex_index * 3) + 0];
		float y = attribs.vertices[(vData.vertex_index * 3) + 1];
		float z = attribs.vertices[(vData.vertex_index * 3) + 2];

		float Nx = 0;//attribs.normals[(vData.normal_index * 3) + 0];
		float Ny = 1.f;//attribs.normals[(vData.normal_index * 3) + 1];
		float Nz = 0;//attribs.normals[(vData.normal_index * 3) + 2];

		float u = 0;//attribs.texcoords[(vData.texcoord_index * 2) + 0];
		float v = 0;//attribs.texcoords[(vData.texcoord_index * 2) + 1];

		_gl_vert_data.push_back(x);
		_gl_vert_data.push_back(y);
		_gl_vert_data.push_back(z);
		_gl_vert_data.push_back(Nx);
		_gl_vert_data.push_back(Ny);
		_gl_vert_data.push_back(Nz);
		_gl_vert_data.push_back(u);
		_gl_vert_data.push_back(v);

		Vertex vert;
		vert.position = { x, y, z };
		vert.normal = { Nx, Ny, Nz };
		vert.uv = { u, v };
		_vertex_data.push_back(vert);
	}

	GraphicsSystem::SubmitMeshForUpload(this);

	return true;
}

void Mesh::LogGLErrorsIfExists(String prefix)
{
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		String errtext;
		switch (error) {
		case GL_INVALID_ENUM: errtext = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: errtext = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: errtext = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW: errtext = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW: errtext = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY: errtext = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: errtext = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: errtext = "UNKNOWN"; break;
		}
		Logger::LogError("GL Error (" + prefix + "):" + errtext);
		error = glGetError();
	}
}

bool Mesh::LoadFromVertexData()
{
	if (_vertex_data.size() <= 0) {
		Logger::LogWarning("Has no existing vertex data");
		return false;
	}

	for (int i = 0; i < _vertex_data.size(); i++) {
		/* Load X, Y, Z, Nx, Ny, Nz, U, and V data */
		float x = _vertex_data[i].position.x;
		float y = _vertex_data[i].position.y;
		float z = _vertex_data[i].position.z;

		float Nx = 0;//attribs.normals[(vData.normal_index * 3) + 0];
		float Ny = 1.f;//attribs.normals[(vData.normal_index * 3) + 1];
		float Nz = 0;//attribs.normals[(vData.normal_index * 3) + 2];

		float u = 0;//attribs.texcoords[(vData.texcoord_index * 2) + 0];
		float v = 0;//attribs.texcoords[(vData.texcoord_index * 2) + 1];

		_gl_vert_data.push_back(x);
		_gl_vert_data.push_back(y);
		_gl_vert_data.push_back(z);
		_gl_vert_data.push_back(Nx);
		_gl_vert_data.push_back(Ny);
		_gl_vert_data.push_back(Nz);
		_gl_vert_data.push_back(u);
		_gl_vert_data.push_back(v);
	}

	GraphicsSystem::SubmitMeshForUpload(this);

	return true;
}
