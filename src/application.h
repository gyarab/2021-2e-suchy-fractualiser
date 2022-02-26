#ifndef FRACTUALISER_APPLICATION_H
#define FRACTUALISER_APPLICATION_H

#include "shader.h"
#include <GLFW/glfw3.h>

class Application {

    static constexpr float VERTICES[] = {
        -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,  1.0f,  0.0f,
        1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
    };

  public:
    Application();
    void main_loop(GLFWwindow *window, Shader* sh);
    void run(GLFWwindow *window);
};

#endif
