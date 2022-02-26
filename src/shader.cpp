#include "shader.h"
#include <iostream>

Shader::Shader(const std::string &fragmentShaderSource) {
    ID = glCreateProgram();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &defaultVertexShader, nullptr);
    glCompileShader(vertexShader);
    if (!compiled(vertexShader))
        std::cerr << "Failed to compile vertex shader" << std::endl;
    const char *source = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &source, nullptr);
    glCompileShader(fragmentShader);
    if (!compiled(fragmentShader))
        std::cerr << "Failed to compile fragment shader" << std::endl;
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        std::cerr << "Failed to link shader program" << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setDouble(const std::string &name, double value) const {
    glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setIntVec(const std::string &name, int value0, int value1) const {
    glUniform2i(glGetUniformLocation(ID, name.c_str()), value0, value1);
}

void Shader::use() const { glUseProgram(ID); }

bool Shader::compiled(const unsigned int shaderId) {
    int success;
    char infolog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (success)
        return success;
    glGetShaderInfoLog(shaderId, 512, nullptr, infolog);
    std::cout << infolog << std::endl;
    return success;
}
