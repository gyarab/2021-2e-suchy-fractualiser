
#include "shader.h"
#include "application.h"
#include <iostream>
#include <sstream>
#include <fstream>

Application::Application() {
}

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    std::cout << "changed size" << std::endl;
    glViewport(0, 0, width, height);
}

void Application::main_loop(GLFWwindow *window, Shader *sh) {
    int iterations = 30;
    double offsetx = 0;
    double offsety = 0;
    double zoom = 1;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
            iterations++;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            iterations--;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            offsetx += 0.01f * zoom;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            offsetx -= 0.01f * zoom;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            offsety += 0.01f * zoom;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            offsety -= 0.01f * zoom;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            zoom -= 0.1f * zoom;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            zoom += 0.1f * zoom;
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            std::cout << zoom << std::endl;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int dimensions[4];
        glGetIntegerv(GL_VIEWPORT, dimensions);
        sh->setInt("width", dimensions[3]);
        sh->setInt("iterations", iterations);
        sh->setDouble("offsetx", offsetx);
        sh->setDouble("offsety", offsety);
        sh->setDouble("zoom", zoom);
        sh->use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::run(GLFWwindow* window) {
    glViewport(0, 0, 800, 800);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    std::ifstream shader_file("shader.glsl");
    std::stringstream ss{};

    ss << shader_file.rdbuf();

    std::string fragmentShaderSource = ss.str();



    auto* sh = new Shader(fragmentShaderSource);

    // create an object that covers the whole screen so the fragment shader runs
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);

    main_loop(window, sh);
}
