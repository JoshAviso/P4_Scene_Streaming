
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


    // Camera Setup
    Object* cam = ObjectManager::RegisterObject(new Object("MainCamera"));
    cam->AddComponent(new CameraComponent("MainCam",
        new Camera::PerspectiveProjection(45.f, Application::GetWindow()->AspectRatio(), 0.01, 200.f)));
    cam->AddComponent(new CameraController(1.f, 360.f));

    Application::Run();
	//RunClient("Frankling", "localhost:50051", true);

    return 0;
}
