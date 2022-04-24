#ifndef FRACTUALISER_APPLICATION_H
#define FRACTUALISER_APPLICATION_H

#include "shader.h"
#include <GLFW/glfw3.h>

struct FractalSettings {
    int iterations = 30;
    double zoom = 5.0;
    double offsetX = -0.8;
    double offsetY = 0.0;
    // int iterations = 769;
    // double zoom = 0.00000000000005276;
    // double offsetX = -1.49485999999895625;
    // double offsetY = 0.0;
};

struct InputSettings {
    int deltaIter = 0;
    double deltaZoom = 0;
    double deltaOffsetX = 0;
    double deltaOffsetY = 0;
    double mouseDeltaX = 0;
    double mouseDeltaY = 0;
    bool printImage = false;
};

struct MouseState {
    bool lmbPressed = false;
    bool rmbPressed = false;
    bool mmbPressed = false;
    double posX = 0;
    double posY = 0;
    double deltaZoom = 0;
};

struct Performance {
    long long timeToRender = 0;
};

class Application {

    static constexpr float VERTICES[] = {
        -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 1.0f,  1.0f,  0.0f,
        1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f, -1.0f, -1.0f, 0.0f,
    };

    Performance performance;
    FractalSettings fractalSettings;
    InputSettings inputSettings;
    MouseState mouseState;
    GLFWwindow *window;

    int windowWidth;
    int windowHeight;
    int bigRenderMultiplier = 8;
    std::string colorFilePath;

  public:
    Application(GLFWwindow *window, int bigRenderMultiplier, std::string &colorFilePath);
    ~Application();
    void mainLoop(Shader &sh, unsigned int VBO);
    void run(std::string &formula);
    void handleKeyInput(int key, int action);
    void handleCursorMovement(double xpos, double ypos);
    void handleMouseInput(int button, int action, int mods);
    void handleScrollInput(double xoffset, double yoffset);
    void writeBMPFromFrameBuffer(std::ofstream &file);
};

#endif
