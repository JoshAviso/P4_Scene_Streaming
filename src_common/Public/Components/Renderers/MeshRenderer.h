#pragma once

#include <Components/Renderers/IRenderer.h>
#include <Common.hpp>
#include <Resources/Mesh.h>
#include <Graphics/Shader.h>
#include <String.hpp>
#include <Threading/IThreadFinishedCallback.h>

class MeshRenderer : public IRenderer, public IThreadFinishedCallback {
public:
	Shared<Mesh> ActiveMesh;
	Shared<Shader> Shader;

private:
	String _asyncResourceName = String();
	String _asyncResourcePath = String();
	String _assignedScene = String();
	float _loadDelay = 0;

public:
	MeshRenderer() {};
	MeshRenderer(String asyncResourceName, String asyncResourcePath, uint loadDelay = 0, String assignedScene = String());

	void BeginLoad();
	void Unload();

	void OnThreadFinished(int id) override;

private:
	void Render(Camera* camera) override;
	friend class ObjectManager;

};