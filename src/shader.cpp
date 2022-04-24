#include "shader.h"
#include <iostream>

Shader::Shader(const std::string &fragmentShaderSource, const std::string &formula) {
    ID = glCreateProgram();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &defaultVertexShader, nullptr);
    glCompileShader(vertexShader);
    if (!compiled(vertexShader))
        std::cerr << "Failed to compile vertex shader" << std::endl;

    const std::string x = fabricateShaderCode(fragmentShaderSource, formula);
    const char *source = x.c_str();
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

/**
 * Expression parser that supports addition, subtratction, multiplication and division
 */
std::string evalExpr(const std::string &expression) {
    if (expression.empty())
        return expression;
    int length = expression.length();

    // check for brackets
    int bracket_count = 0;
    if (expression.at(0) == '(') {
        for (int i = 0; i < length - 1; i++) {
            char c = expression.at(i);
            if (c == '(')
                bracket_count++;
            else if (c == ')')
                bracket_count--;
            if (bracket_count == 0)
                break;
        }
    }
    if (bracket_count == 1)
        return "(" + evalExpr(expression.substr(1, length - 2)) + ")";

    // find split
    int highest_type = 0;
    int index = -1;
    bracket_count = 0;
    for (int i = 0; i < length; i++) {
        char c = expression.at(i);
        int value = -1;
        switch (c) {
        case '(':
            bracket_count++;
            break;
        case ')':
            bracket_count--;
            break;
        case '*':
        case '/':
            value = 1;
            break;
        case '+':
        case '-':
            value = 2;
            break;
        default:
            value = -1;
        }
        if (bracket_count > 0)
            continue;
        if (value >= highest_type) {
            highest_type = value;
            index = i;
        }
    }

    // take care of primitives
    if (index == -1) {
        if (std::isalpha(expression.at(0))) {
            return expression;
        } else if (std::isdigit(expression.at(0))) {
            if (expression.at(length - 1) == 'i') {
                return "dvec2(0, " + expression.substr(0, length - 1) + ")";
            } else {
                return "dvec2(" + expression + ",0)";
            }
        }
    }

    // recurse into sub-expressions
    std::string left = evalExpr(expression.substr(0, index));
    std::string right = evalExpr(expression.substr(index + 1));
    switch (expression.at(index)) {
    case '*':
        return "mult(" + left + "," + right + ")";
    case '/':
        return "div(" + left + "," + right + ")";
    case '+':
        return left + "+" + right;
    case '-':
        return left + "-" + right;
    }
    return "";
}

std::string Shader::fabricateShaderCode(std::string src, const std::string &formula) {
    std::string e = evalExpr(formula);
    int index = src.find("%formula%");
    if (index < 0)
        return "";
    src.replace(index, 9, "z=" + e + ";");
    return src;
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
