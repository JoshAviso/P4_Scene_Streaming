#include <Components/Renderers/MeshRenderer.h>
#include <Objects/Object.h>
#include <Camera/Camera.h>
#include <Logger.hpp>

void MeshRenderer::Render(Camera* camera)
{

	if (Mesh == nullptr) return;
	if (!Mesh->_loaded) return;
	if (Shader == nullptr) return;
	if (!Shader->_linked) return;

	//Logger::Log(camera->GetProjection());
	Shader->PassData("view", camera->GetViewMat());
	Shader->PassData("projection", camera->GetProjection());
	Shader->PassData("transform", _owner->transform);
	Shader->PassData("base_color", Vec3(base_color));

	Shader->Use();
	glBindVertexArray(Mesh->_vao);
	glDrawArrays(GL_TRIANGLES, 0, Mesh->_vertex_data.size());
	glBindVertexArray(0);
}
