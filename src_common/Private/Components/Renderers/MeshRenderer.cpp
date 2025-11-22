#include <Components/Renderers/MeshRenderer.h>

void MeshRenderer::Render()
{
	if (Mesh == nullptr) return;
	if (!Mesh._loaded) return;
	if (Shader == nullptr) return;
	if (!Shader._linked) return;

	Shader->PassData("transform", Mat4(_owner->transform));
	Shader->PassData("base_color", Vec3(base_color));

	Shader->Use();
	glBindVertexArray(Mesh->_vao);
	glDrawArrays(GL_TRIANGLES, 0, Mesh->_vertex_data.size());
	glBindVertexArray(0);
}
