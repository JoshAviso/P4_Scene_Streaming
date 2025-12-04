
#include "SceneStreaming_Client.h"

#include <Application.h>
#include <Window/Window.h>

#include <Objects/ObjectManager.h>
#include <Objects/Object.h>
#include <Components/Update/CameraComponent.h>
#include <Components/Update/CameraController.h>

#include <UserInterface/UserInterfaceManager.h>
#include <UserInterface/CustomUI/SceneSelectUI.h>

#include <Networking/SceneStreamClient.h>

int main()
{
    SceneStreamClient::Initialize("localhost:50051");

    Application* app = Application::Initialize({
        120.f,
        new Window({
                "P4 Scene Streaming Client | Aviso & Taylan",
                800, 600,
                Color::Black()
            }),
        SceneManager::Desc{
            false, "MainCamera"
        }
    });

    // Camera Setup
    Object* cam = ObjectManager::RegisterObject(new Object("MainCamera"));
    cam->AddComponent(new CameraComponent("MainCam", new Camera::PerspectiveProjection(45.f, Application::GetWindow()->AspectRatio(), 0.01, 200.f)));
    cam->AddComponent(new CameraController(1.f, 360.f));
    cam->transform.position = Vec3(0.f, 0.f, 1.0f);

    UserInterfaceManager::AddUI(new SceneSelectUI());

    Application::Run();

    return 0;
}
