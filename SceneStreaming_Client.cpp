
#include "SceneStreaming_Client.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <tiny_obj_loader.h>

#include <iostream>
#include <vector>

#include <Application.h>
#include <Window/Window.h>

#include <Resources/ResourceManager.h>
#include <Resources/Mesh.h>
#include <Resources/IShaderResource.h>
#include <Objects/ObjectManager.h>
#include <Objects/Object.h>
#include <Graphics/ShaderList.h>
#include <Components/Renderers/MeshRenderer.h>
#include <Components/Update/CameraComponent.h>
#include <Components/Update/CameraController.h>

int main()
{
    Application* app = Application::Initialize({
        new Window({
                "P4 Scene Streaming | Aviso & Taylan",
                800, 600,
                Color::Black()
            }),
        SceneManager::Desc{
            true, "MainCamera"
        }
        });

    Shared<Mesh> bunnyMesh = ResourceManager::LoadFromFile<Mesh>("Bunny", "Assets/Models/bunny.obj");
    Shared<Mesh> teapotMesh = ResourceManager::LoadFromFile<Mesh>("Teapot", "Assets/Models/teapot.obj");
    Shared<Shader> basicShader = ShaderList::GenerateShader("Basic Shader", 
        ResourceManager::LoadFromFile<VertexShader>("Basic Vertex Shader", "Assets/Shaders/sample.vert"),
        ResourceManager::LoadFromFile<FragShader>("Basic Frag Shader", "Assets/Shaders/sample.frag")
    );

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Object* bunny = ObjectManager::RegisterObject(new Object("Bunny" + std::to_string(i * j)));
            MeshRenderer* bunnyRenderer = bunny->AddComponent(new MeshRenderer());
            bunnyRenderer->Mesh = bunnyMesh;
            bunnyRenderer->Shader = basicShader;
            bunnyRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            bunny->transform.position = Vec3((i - 2) * 0.5f, 0.f, (j - 2) * 0.5f);
            bunny->transform.rotation = Quaternion((i + j) / 10.f * 360.f, Vec3(0.f, 1.f, 0.f));
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Object* teapot = ObjectManager::RegisterObject(new Object("Teapot" + std::to_string(i * j)));
            MeshRenderer* tpRenderer = teapot->AddComponent(new MeshRenderer());
            tpRenderer->Mesh = teapotMesh;
            tpRenderer->Shader = basicShader;
            tpRenderer->base_color = Color((float)i / 5.f, (float)j / 5.f, 0.2f, 1.f);
            teapot->transform.position = Vec3((i - 2) * 0.5f, -0.5f, (j - 2) * 0.5f);
            teapot->transform.scale = Vec3(0.05f, 0.05f, 0.05f);
            teapot->transform.rotation = Quaternion((i + j) / 10.f * -360.f, Vec3(0.f, 1.f, 0.f));
        }
    }

    // Camera Setup
    Object* cam = ObjectManager::RegisterObject(new Object("MainCamera"));
    cam->AddComponent(new CameraComponent("MainCam",
        new Camera::PerspectiveProjection(45.f, Application::GetWindow()->AspectRatio(), 0.01, 200.f)));
    cam->AddComponent(new CameraController(1.f, 360.f));

    Application::Run();
	//RunClient("Frankling", "localhost:50051", true);

    return 0;
}
