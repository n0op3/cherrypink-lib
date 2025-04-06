#include "glad/glad.h"

#include "rendering/window.hpp"
#include "tiny_cherno.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace tiny_cherno {

void error_callback(int error_code, const char *description) {
    spdlog::error("GLFW error {}:", error_code, description);
}

static TinyChernoRuntime *s_runtime = nullptr;

InitializationError init(WindowParameters &window_parameters) {
    if (s_runtime) {
        return NONE;
    }

    spdlog::info("Loading the TinyCherno runtime!");
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        spdlog::critical("Failed to initialize GLFW");
        return GLFW_FAILED;
    }

    if (!window_parameters.resizeable) {
        glfwWindowHint(GLFW_RESIZABLE, false);
    }

    GLFWwindow *window =
        glfwCreateWindow(window_parameters.width, window_parameters.height,
                         window_parameters.title, NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        spdlog::critical("Failed to load OpenGL symbols");
        return GLAD_FAILED;
    }

    s_runtime = new TinyChernoRuntime(window);
    return NONE;
}

void run() {
    TinyChernoRuntime::GetRuntime()->Run();
    delete s_runtime;
}

void TinyChernoRuntime::Run() {
    glfwWindowHint(GLFW_VISIBLE, true);

    spdlog::info("Entering the main loop...");
    while (!glfwWindowShouldClose(m_window)) {
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

TinyChernoRuntime::TinyChernoRuntime(GLFWwindow *window) : m_window(window) {}

TinyChernoRuntime *TinyChernoRuntime::GetRuntime() { return s_runtime; }

} // namespace tiny_cherno
