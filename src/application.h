#ifndef FRACTUALISER_APPLICATION_H
#define FRACTUALISER_APPLICATION_H

#include "shader.h"
#include <GLFW/glfw3.h>

struct FractalSettings {
    int iterations = 30;
    double zoom = 5.0;
    double offsetX = -0.8;
    double offsetY = 0.0;
};

struct InputSettings {
    int deltaIter = 0;
    double deltaZoom = 0;
    double deltaOffsetX = 0;
    double deltaOffsetY = 0;
};

class Application {

    static constexpr float VERTICES[] = {
        -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,  1.0f,  0.0f,
        1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
    };

    FractalSettings *fractalSettings;
    InputSettings *inputSettings;
    GLFWwindow *window;

  public:
    Application(GLFWwindow *window);
    ~Application();
    void mainLoop(Shader *sh);
    void run();
    void handleInput(int key, int action);
};

#endif
