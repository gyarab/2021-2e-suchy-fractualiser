#ifndef FRACTUALISER_SHADER_H
#define FRACTUALISER_SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
    const char *defaultVertexShader = "#version 330 core\n"
                                      "layout (location = 0) in vec3 aPos;\n"
                                      "void main()\n"
                                      "{\n"
                                      "   gl_Position = vec4(aPos, 1.0);\n"
                                      "}\0";

    static bool compiled(unsigned int shaderId);

  public:
    explicit Shader(const std::string &fragmentShaderSource);

    unsigned int ID;

    void use() const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setDouble(const std::string &name, double value) const;

    void setIntVec(const std::string &name, int value0, int value1) const;
};

#endif // FRACTUALISER_SHADER_H
