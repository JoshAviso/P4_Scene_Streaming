
#include "SceneStreaming_Client.h"

#include <glad/glad.h> // OpenGL loader
#include <GLFW/glfw3.h> // Window/context

#include <iostream>

int main()
{
	//RunClient("Frankling", "localhost:50051", true);

    // Initialize GLFW library
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create window with OpenGL context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Set clear color to cornflower blue
        glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup GLFW resources
    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}
