#include "application.h"
#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

Application::Application(GLFWwindow *window) {
    fractalSettings = new FractalSettings{};
    this->window = window;
}

Application::~Application() {
    delete fractalSettings;
    std::cout << "Cleaned up application" << std::endl;
}

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::handleInput() {}

void Application::mainLoop(Shader *sh) {
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            fractalSettings->iterations++;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            fractalSettings->iterations--;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            fractalSettings->offset_x += 0.01f * fractalSettings->zoom;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            fractalSettings->offset_x -= 0.01f * fractalSettings->zoom;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            fractalSettings->offset_y += 0.01f * fractalSettings->zoom;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            fractalSettings->offset_y -= 0.01f * fractalSettings->zoom;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            fractalSettings->zoom -= 0.1f * fractalSettings->zoom;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            fractalSettings->zoom += 0.1f * fractalSettings->zoom;
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            std::cout << "zoom: " << fractalSettings->zoom << std::endl;
            std::cout << "iterations: " << fractalSettings->iterations << std::endl;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int dimensions[4];
        glGetIntegerv(GL_VIEWPORT, dimensions);
        sh->setIntVec("dimensions", dimensions[2], dimensions[3]);
        sh->setInt("iterations", fractalSettings->iterations);
        sh->setDouble("offsetx", fractalSettings->offset_x);
        sh->setDouble("offsety", fractalSettings->offset_y);
        sh->setDouble("zoom", fractalSettings->zoom);
        sh->use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
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

    auto *sh = new Shader(fragmentShaderSource);

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, texture);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char data[] = {0xe7, 0x6f, 0x51, 0xf4, 0xa2, 0x61, 0xe9, 0xc4, 0x6a, 0x2a, 0x9d, 0x8f, 0x26, 0x46, 0x53};

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, sizeof(data) / 3, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    sh->setInt("texture1", 0);
    mainLoop(sh);

    delete sh;
}
