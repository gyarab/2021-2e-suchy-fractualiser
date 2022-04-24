#include "application.h"
#include "args.h"
#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char **argv) {
    Args::Parser parser(argc, argv);
    int multiplier;
    std::string formula;
    std::string colorFilePath;
    try {
        bool help = parser.get_arg<bool>({.long_name = "help", .short_name = 'h'}, false);
        if (help) {
            std::cout << "Usage: fractualiser [options] [formula]" << std::endl
                      << "  -h, --help       Shows this help message" << std::endl
                      << "  -m, --multiplier How many times should a big render be bigger (default: 4)" << std::endl
                      << "  -c, --colors     Path to a file with newline seperated list" << std::endl
                      << "                   of hex colors (#ffffff)" << std::endl
                      << std::endl
                      << "  formula          The formula to calculate (default: z*z+c)" << std::endl
                      << "                   Supported operations: +-*/ with complex numbers" << std::endl
                      << "                   c - position of the current pixel in the complex plane" << std::endl
                      << "                   z - the iterated and final computed value" << std::endl
                      << std::endl
                      << "Controls: Use your mouse or ASDF (E-zoom in, Q-zoom out) to control the camera." << std::endl
                      << "  F - increase iteration count, C - decrase iteration count" << std::endl
                      << "  G - print debug information and camera position" << std::endl
                      << "  P - create a big render - the output is in image.bmp" << std::endl
                      << "  ESC - exit the program" << std::endl
                      << std::endl
                      << "Fractualiser (fract(al)(vis)ualiser) is a program to render fractals using" << std::endl
                      << "OpenGL on a GPU." << std::endl;
            return 0;
        }
        multiplier = parser.get_arg<int>({.long_name = "multiplier", .short_name = 'm'}, 4);
        colorFilePath = parser.get_arg<std::string>({.long_name = "colors", .short_name = 'c'}, "");
        formula = parser.get_arg<std::string>({.position = 1, .long_name = "formula"}, "z*z+c");
    } catch (Args::Exceptions::ArgumentParsingError &e) {
        std::cout << e.what() << std::endl;
        std::exit(1);
    }
    if (!glfwInit()) {
        std::cout << "Failed to initialise GLFW" << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "fractals-distinct-title", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }

    Application app = Application(window, multiplier, colorFilePath);

    app.run(formula);

    glfwTerminate();
}
