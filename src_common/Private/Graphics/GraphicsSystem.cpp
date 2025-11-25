#include <Graphics/GraphicsSystem.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdexcept>
#include <Application.h>

#include <Objects/ObjectManager.h>
#include <Window/Window.h>
#include <Camera/CameraManager.h>
#include <Camera/Camera.h>
#include <Graphics/ShaderList.h>

void GraphicsSystem::Render(Window* window)
{
    int display_w, display_h;
    glfwGetFramebufferSize(window->_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    Color clearColor = window->_desc.clearColor;
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply Camera
    Shared<Camera> cam = CameraManager::GetActiveCam();
    if (cam == nullptr) {
        cam = Make_Shared<Camera>();
        cam->projection = new Camera::PerspectiveProjection(100.f, Application::GetWindow()->AspectRatio(), 0.01f, 200.f);
    }

    //ImGui::NewFrame();
    // TODO : Render UI Step
    ObjectManager::_instance->RenderObjects(cam.get());

    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window->_window);
}

// SINGLETON 
GraphicsSystem* GraphicsSystem::_instance = nullptr;
GraphicsSystem* GraphicsSystem::Initialize()
{
    if (_instance != nullptr) return _instance;
    
    if (!glfwInit()) throw std::runtime_error("Failed to initialize glfw!");

    _instance = new GraphicsSystem();
    return _instance;
}

GraphicsSystem::GraphicsSystem()
{
    // Initializations
    if (!Application::GetWindow()->Initialize()) throw std::runtime_error("Window failed to initialize!");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("Failed to initialize glad!");
    glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(Application::GetWindow()->_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();
}

GraphicsSystem::~GraphicsSystem()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}
