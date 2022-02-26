#include "application.h"
#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "fractals-distinct-title", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    Application *app = new Application(window);
    app->run();

    delete app;

    glfwTerminate();
}
