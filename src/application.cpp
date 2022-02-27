#include "application.h"
#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Application::Application(GLFWwindow *window) {
    fractalSettings = new FractalSettings;
    inputSettings = new InputSettings;
    performance = new Performance;
    this->window = window;
    glfwSetWindowUserPointer(window, this);
}

Application::~Application() {
    delete fractalSettings;
    delete inputSettings;
    delete performance;
    glfwSetWindowUserPointer(this->window, nullptr);
    std::cout << "Cleaned up application" << std::endl;
}

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    // the function passed to glfwSetKeyCallback must be static, so we reroute it to our application method
    ((Application *)glfwGetWindowUserPointer(window))->handleInput(key, action);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::handleInput(int key, int action) {

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
        fractalSettings->offsetX += inputSettings->deltaOffsetX * fractalSettings->zoom;
        fractalSettings->offsetY += inputSettings->deltaOffsetY * fractalSettings->zoom;
        fractalSettings->zoom += inputSettings->deltaZoom * fractalSettings->zoom;

        // set shader uniforms
        int dimensions[4];
        glGetIntegerv(GL_VIEWPORT, dimensions);

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
