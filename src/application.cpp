#include "application.h"
#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Application::Application(GLFWwindow *window) {
    fractalSettings = new FractalSettings;
    inputSettings = new InputSettings;
    performance = new Performance;
    mouseState = new MouseState;
    this->window = window;
    glfwSetWindowUserPointer(window, this);
}

Application::~Application() {
    delete fractalSettings;
    delete inputSettings;
    delete performance;
    delete mouseState;
    glfwSetWindowUserPointer(this->window, nullptr);
    std::cout << "Cleaned up application" << std::endl;
}

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    // the function passed to glfwSetKeyCallback must be static, so we reroute it to our application method
    ((Application *)glfwGetWindowUserPointer(window))->handleKeyInput(key, action);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    ((Application *)glfwGetWindowUserPointer(window))->handleCursorMovement(xpos, ypos);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    ((Application *)glfwGetWindowUserPointer(window))->handleMouseInput(button, action, mods);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    ((Application *)glfwGetWindowUserPointer(window))->handleScrollInput(xoffset, yoffset);
}

void Application::handleCursorMovement(double xpos, double ypos) {
    double deltax = xpos - mouseState->posX;
    double deltay = ypos - mouseState->posY;
    mouseState->posX = xpos;
    mouseState->posY = ypos;
    if (mouseState->lmbPressed) {
        inputSettings->mouseDeltaX += deltax * fractalSettings->zoom / windowHeight;
        inputSettings->mouseDeltaY += deltay * fractalSettings->zoom / windowHeight;
    }
}

void Application::handleMouseInput(int key, int action, int mods) {
    if (action == GLFW_REPEAT)
        return;
    switch (key) {
    case GLFW_MOUSE_BUTTON_LEFT:
        mouseState->lmbPressed = action == GLFW_PRESS;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mouseState->rmbPressed = action == GLFW_PRESS;
        mouseState->deltaZoom = 0;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mouseState->mmbPressed = action == GLFW_PRESS;
    }
}

void Application::handleScrollInput(double xoffset, double yoffset) {
    double deltazoom = 0.1 * yoffset * fractalSettings->zoom;
    fractalSettings->offsetX += (mouseState->posX - windowWidth / 2.0) * deltazoom / windowHeight;
    fractalSettings->offsetY -= (mouseState->posY - windowHeight / 2.0) * deltazoom / windowHeight;
    fractalSettings->zoom -= deltazoom;
}

void Application::handleKeyInput(int key, int action) {

    int k;
    switch (action) {
    case GLFW_PRESS:
        k = 1;
        break;
    case GLFW_RELEASE:
        k = -1;
        break;
    default:
        return; // this is GLFW_REPEAT, we ignore this event
    }

    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_W:
        inputSettings->deltaOffsetY += 0.01 * k;
        break;
    case GLFW_KEY_S:
        inputSettings->deltaOffsetY += -0.01 * k;
        break;
    case GLFW_KEY_A:
        inputSettings->deltaOffsetX += -0.01 * k;
        break;
    case GLFW_KEY_D:
        inputSettings->deltaOffsetX += 0.01 * k;
        break;
    case GLFW_KEY_E:
        inputSettings->deltaZoom += -0.1 * k;
        break;
    case GLFW_KEY_Q:
        inputSettings->deltaZoom += 0.1 * k;
        break;
    case GLFW_KEY_F:
        inputSettings->deltaIter += k;
        break;
    case GLFW_KEY_C:
        inputSettings->deltaIter += -k;
        break;
    case GLFW_KEY_G:
        if (action == GLFW_PRESS) {
            std::cout << "============================================" << std::endl;
            std::cout << "deltaOffsetX " << inputSettings->deltaOffsetX << "\n"
                      << "deltaOffsetY " << inputSettings->deltaOffsetY << "\n"
                      << "deltaIter    " << inputSettings->deltaIter << "\n"
                      << "deltaZoom    " << inputSettings->deltaZoom << "\n"
                      << std::endl;

            std::cout.precision(17);
            std::cout << "offsetX " << fractalSettings->offsetX << "\n"
                      << "offsetY " << fractalSettings->offsetY << "\n"
                      << "iter    " << fractalSettings->iterations << "\n"
                      << "zoom    " << fractalSettings->zoom << "\n"
                      << std::endl;
            std::cout << "time to render: " << performance->timeToRender << "\n" << std::endl;
        }
        break;

    default:
        std::cout << "ignoring key" << std::endl;
    }
}

void Application::mainLoop(Shader *sh) {
    clock_t start, end;
    while (!glfwWindowShouldClose(window)) {
        start = clock();

        // update viewport according to input
        fractalSettings->iterations += inputSettings->deltaIter;
        fractalSettings->offsetX += inputSettings->deltaOffsetX * fractalSettings->zoom - inputSettings->mouseDeltaX;
        fractalSettings->offsetY += inputSettings->deltaOffsetY * fractalSettings->zoom + inputSettings->mouseDeltaY;
        fractalSettings->zoom += (inputSettings->deltaZoom) * fractalSettings->zoom;

        inputSettings->mouseDeltaX = 0;
        inputSettings->mouseDeltaY = 0;

        // get viewport dimensions
        int dimensions[4];
        glGetIntegerv(GL_VIEWPORT, dimensions);
        windowWidth = dimensions[2];
        windowHeight = dimensions[3];

        // set shader uniforms
        sh->setIntVec("dimensions", dimensions[2], dimensions[3]);
        sh->setInt("iterations", fractalSettings->iterations);
        sh->setDouble("offsetx", fractalSettings->offsetX);
        sh->setDouble("offsety", fractalSettings->offsetY);
        sh->setDouble("zoom", fractalSettings->zoom);

        // draw
        sh->use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
        end = clock();
        performance->timeToRender = end - start;
    }
}

void Application::run() {
    glViewport(0, 0, 800, 800);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // preload current cursor position
    glfwGetCursorPos(window, &mouseState->posX, &mouseState->posY);

    std::ifstream shader_file("shader.glsl");
    std::stringstream ss{};

    ss << shader_file.rdbuf();

    std::string fragmentShaderSource = ss.str();

    std::string formula = "z*z-0.8+0.156i";

    auto *sh = new Shader(fragmentShaderSource, formula);

    // create an object that covers the whole screen so the fragment shader runs
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_1D, texture);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char data[] = {0xe7, 0x6f, 0x51, 0xf4, 0xa2, 0x61, 0xe9, 0xc4, 0x6a, 0x2a, 0x9d, 0x8f, 0x26, 0x46, 0x53};

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, sizeof(data) / 3, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    mainLoop(sh);

    delete sh;
}
