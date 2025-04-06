#include "tiny_cherno.hpp"
#include "glad/glad.h"
#include "rendering/window.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

using namespace tiny_cherno;

void error_callback(int error_code, const char *description) {
    spdlog::error("GLFW error {}:", error_code, description);
}

InitializationError tiny_cherno::init(WindowParameters &window_parameters) {
    spdlog::info("Loading the TinyCherno runtime!");
    glfwSetErrorCallback(error_callback);

    spdlog::info("Initializing GLFW...");
    if (!glfwInit()) {
        spdlog::critical("Failed to initialize GLFW");
        return GLFW_FAILED;
    }

    spdlog::info("Creating a GLFW window...");

    if (!window_parameters.resizeable) {
        spdlog::info("Disabling window resizing");
        glfwWindowHint(GLFW_RESIZABLE, false);
    }

    GLFWwindow *window =
        glfwCreateWindow(window_parameters.width, window_parameters.height,
                         window_parameters.title, NULL, NULL);

    spdlog::info("Making the window the current OpenGL context");
    glfwMakeContextCurrent(window);

    spdlog::info("Loading OpenGL symbols");
    if (!gladLoadGL()) {
        spdlog::critical("Failed to load OpenGL symbols");
        return GLAD_FAILED;
    }

    spdlog::info("Displaying the window");
    glfwWindowHint(GLFW_VISIBLE, true);

    spdlog::info("Entering the window loop...");
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return NONE;
}
