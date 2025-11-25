#include <Resources/Mesh.h>

#include <tiny_obj_loader.h>

#include <Common.hpp>
#include <Logger.hpp>

Mesh::~Mesh()
{
	if (_loaded) {
		glDeleteVertexArrays(1, &_vao);
		glDeleteBuffers(1, &_vbo);
	}
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

	List<GLfloat> vert_data;
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

		vert_data.push_back(x);
		vert_data.push_back(y);
		vert_data.push_back(z);
		vert_data.push_back(Nx);
		vert_data.push_back(Ny);
		vert_data.push_back(Nz);
		vert_data.push_back(u);
		vert_data.push_back(v);

		Vertex vert;
		vert.position = { x, y, z };
		vert.normal = { Nx, Ny, Nz };
		vert.uv = { u, v };
		_vertex_data.push_back(vert);
	}

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(GLfloat) * vert_data.size(), 
		vert_data.data(),
		GL_DYNAMIC_DRAW
	);

	int vertex_size = 8;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLintptr norm_stride = 3 * sizeof(float);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)norm_stride);
	glEnableVertexAttribArray(1);

	GLintptr uv_stride = 2 * sizeof(float);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)uv_stride);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_loaded = true;

	return true;
}
