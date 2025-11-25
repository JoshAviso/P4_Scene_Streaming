
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
            })
        });

    // Bunny Setup
    Shared<Mesh> bunnyMesh = ResourceManager::LoadFromFile<Mesh>("Bunny", "Assets/bunny.obj");
    Shared<Shader> basicShader = ShaderList::GenerateShader("Basic Shader", 
        ResourceManager::LoadFromFile<VertexShader>("Basic Vertex Shader", "Assets/Shaders/sample.vert"),
        ResourceManager::LoadFromFile<FragShader>("Basic Frag Shader", "Assets/Shaders/sample.frag")
    );
    Object* bunny = ObjectManager::RegisterObject(new Object("Bunny"));
    MeshRenderer* bunnyRenderer = bunny->AddComponent(new MeshRenderer());
    bunnyRenderer->Mesh = bunnyMesh;
    bunnyRenderer->Shader = basicShader;
    bunnyRenderer->base_color = Color::Red();
    bunny->transform.position.z = 0.f;

    // Camera Setup
    Object* cam = ObjectManager::RegisterObject(new Object("MainCamera"));
    cam->AddComponent(new CameraComponent("MainCam",
        new Camera::PerspectiveProjection(45.f, Application::GetWindow()->AspectRatio(), 0.01, 200.f)));
    cam->AddComponent(new CameraController(1.f));

    Application::Run();
	//RunClient("Frankling", "localhost:50051", true);

    /*
     // Initialize GLFW
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "TinyObjLoader + OpenGL + ImGui", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    // Load OBJ model using TinyObjLoader
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "Assets/bunny.obj");
    if (!ret) {
        std::cerr << "Failed to load/parse .obj file\n" << "Err: " << err.c_str() << std::endl;
        return -1;
    }

    // Extract vertex buffer (vertices only for simplicity)
    std::vector<Vertex> vertices;
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex v;
            v.x = attrib.vertices[3 * index.vertex_index + 0];
            v.y = attrib.vertices[3 * index.vertex_index + 1];
            v.z = attrib.vertices[3 * index.vertex_index + 2];
            vertices.push_back(v);
        }
    }

    // Create OpenGL VBO for vertices
    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui window
        ImGui::Begin("Hello TinyObjLoader");
        ImGui::Text("Simple OBJ model loaded: %zu vertices", vertices.size());
        ImGui::End();

        // Rendering OpenGL
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
        glBindVertexArray(0);

        // Render ImGui draw data
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    */

    return 0;
}
