#include <Graphics/GraphicsSystem.h>

#include <Resources/Mesh.h>
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
#include <UserInterface/UserInterfaceManager.h>

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

    UserInterfaceManager::BeginFrame();

    ObjectManager::_instance->RenderObjects(cam.get());
    UserInterfaceManager::Render();

    glfwSwapBuffers(window->_window);
}

void GraphicsSystem::SubmitMeshForUpload(Mesh* mesh)
{
    mesh->_loaded = false;
    _instance->_meshesToUpload.push_back(mesh);
}

void GraphicsSystem::UploadSubmittedMeshes()
{
    for (int i = 0; i < _instance->_meshesToUpload.size(); i++) {
        Mesh* mesh = _instance->_meshesToUpload[i];
        if (mesh == nullptr) continue;

        glGenVertexArrays(1, &mesh->_vao);
        LogGLErrorsIfExists("VAO Gen");
        glGenBuffers(1, &mesh->_vbo);
        LogGLErrorsIfExists("VBO Gen");
        glBindVertexArray(mesh->_vao);
        LogGLErrorsIfExists("VAO Bind");
        glBindBuffer(GL_ARRAY_BUFFER, mesh->_vbo);
        LogGLErrorsIfExists("VBO Bind");

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * mesh->_gl_vert_data.size(),
            mesh->_gl_vert_data.data(),
            GL_DYNAMIC_DRAW
        );
        LogGLErrorsIfExists("Buffer Data Copy");

        int vertex_size = 8;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        LogGLErrorsIfExists("Position Attrib pointer");

        GLintptr norm_stride = 3 * sizeof(float);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)norm_stride);
        glEnableVertexAttribArray(1);
        LogGLErrorsIfExists("Normals Attrib pointer");

        GLintptr uv_stride = 2 * sizeof(float);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float), (void*)uv_stride);
        glEnableVertexAttribArray(1);
        LogGLErrorsIfExists("UVs Attrib pointer");

        glBindVertexArray(0);
        LogGLErrorsIfExists("Clear VAO");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        LogGLErrorsIfExists("Clear VBO");

        mesh->_loaded = true;
    }
    _instance->_meshesToUpload.clear();
}

void GraphicsSystem::LogGLErrorsIfExists(const String text)
{
    GLenum error = glGetError();
    while (error != GL_NO_ERROR) {
        String errtext;
        switch (error) {
        case GL_INVALID_ENUM: errtext = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE: errtext = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: errtext = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW: errtext = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW: errtext = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY: errtext = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: errtext = "INVALID_FRAMEBUFFER_OPERATION"; break;
        default: errtext = "UNKNOWN"; break;
        }
        Logger::LogError("GL Error (" + text + "):" + errtext);
        error = glGetError();
    }
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
