#pragma once

#include <Components/Renderers/IRenderer.h>
#include <Common.hpp>
#include <Resources/Mesh.h>
#include <Graphics/Shader.h>

class MeshRenderer : public IRenderer {
public:
	Shared<Mesh> Mesh;
	Shared<Shader> Shader;

private:
	void Render(Camera* camera) override;
	friend class ObjectManager;
};