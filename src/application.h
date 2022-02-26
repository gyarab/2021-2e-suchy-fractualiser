#ifndef FRACTUALISER_APPLICATION_H
#define FRACTUALISER_APPLICATION_H

#include "shader.h"
#include <GLFW/glfw3.h>

struct FractalSettings {
    int iterations = 30;
    double zoom = 5.0;
    double offset_x = -0.8;
    double offset_y = 0.0;
};

class Application {

    static constexpr float VERTICES[] = {
        -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,  1.0f,  0.0f,
        1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
    };

    FractalSettings *fractalSettings;
    GLFWwindow *window;

  public:
    Application(GLFWwindow *window);
    ~Application();
    void mainLoop(Shader *sh);
    void run();
    void handleInput();
};

#endif
