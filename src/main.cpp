#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

// vertices to cover the whole screen (two triangles)
const float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
};

void key_callback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    std::cout << "changed size" << std::endl;
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "fractals-distinct-title", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, 800, 800);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    std::string fragmentShaderSource =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform int width;\n"
            "uniform float iterations;\n"
            "\n"
            "void main() {\n"
            "    int center = width/2;\n"
            "    float x = (gl_FragCoord.x-center)/width*3-0.8;\n"
            "    float y = (gl_FragCoord.y-center)/width*3;\n"
            "    float ogx = x;\n"
            "    float ogy = y;\n"
            "    int i;\n"
            "    bool suc = true;\n"
            "    for (i = 0; i < iterations; i++) {\n"
            "         float tempx = x*x - y*y + ogx;\n"
            "         float tempy = 2*y*x + ogy;\n"
            "         x = tempx;\n"
            "         y = tempy;\n"
            "         if (x*x+y*y > 3000) { suc = false; break; }\n"
            "    }\n"
            "    if (suc) FragColor = vec4(0.7f, 0.0f, 0.0f, 1.0f);\n"
            "    else FragColor = vec4(0.0f, 0.7f, 0.0f, 1.0f);\n"
            "}";

    auto *sh = new Shader(fragmentShaderSource);

    // create an object that covers the whole screen so the fragment shader runs
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    sh->setFloat("iterations", 30);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int dimensions[4];
        glGetIntegerv(GL_VIEWPORT, dimensions);
        sh->setInt("width", dimensions[3]);
        sh->use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}
