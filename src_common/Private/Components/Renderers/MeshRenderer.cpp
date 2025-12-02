#include <Components/Renderers/MeshRenderer.h>
#include <Objects/Object.h>
#include <Camera/Camera.h>
#include <Logger.hpp>
#include <Resources/ResourceManager.h>
#include <Threading/ThreadPoolManager.h>
#include <Threading/WorkerTasks/LoadResourceTask.h>

void MeshRenderer::Render(Camera* camera)
{

	if (ActiveMesh == nullptr) return;
	if (!ActiveMesh->_loaded) return;
	if (Shader == nullptr) return;
	if (!Shader->_linked) return;

	//Logger::Log(camera->GetProjection());
	Shader->PassData("view", camera->GetViewMat());
	Shader->PassData("projection", camera->GetProjection());
	Shader->PassData("transform", _owner->transform);
	Shader->PassData("base_color", Vec3(base_color));

	Shader->Use();
	glBindVertexArray(ActiveMesh->_vao);
	glDrawArrays(GL_TRIANGLES, 0, ActiveMesh->_vertex_data.size());
	glBindVertexArray(0);
}

MeshRenderer::MeshRenderer(String asyncResourceName, String asyncResourcePath) : _asyncResourceName(asyncResourceName)
{
	//ActiveMesh = ResourceManager::LoadFromFile<Mesh>(asyncResourceName, asyncResourcePath);
	Shared<ThreadPool> pool = ThreadPoolManager::GetThreadPool("Main");
	pool->ScheduleTask(new LoadResourceTask<Mesh>(_asyncResourceName, asyncResourcePath, 5000, this));
}

void MeshRenderer::OnThreadFinished(int id)
{
	Shared<Mesh> mesh = ResourceManager::GetResource<Mesh>(_asyncResourceName);
	ActiveMesh = mesh;
}
