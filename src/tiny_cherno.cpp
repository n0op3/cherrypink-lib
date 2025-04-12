#include "glad/glad.h"

#include "rendering/window.hpp"
#include "tiny_cherno.hpp"

#include "GLFW/glfw3.h"
#include "runtime/runtime.hpp"
#include "spdlog/spdlog.h"

namespace tiny_cherno {

static TinyChernoRuntime *s_runtime = nullptr;

InitializationError init(WindowParameters &windowParameters) {
    if (s_runtime)
        return NONE;

    spdlog::info("Initializing the TinyCherno runtime!");
    glfwSetErrorCallback([](int errorCode, const char *description) {
        spdlog::error("GLFW error {}: {}", errorCode, description);}
    );

    if (!glfwInit()) {
        spdlog::critical("Failed to initialize GLFW");
        return GLFW_FAILED;
    }

    if (!windowParameters.resizable)
        glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window =
        glfwCreateWindow(windowParameters.width, windowParameters.height,
                         windowParameters.title, NULL, NULL);

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

TinyChernoRuntime *TinyChernoRuntime::GetRuntime() { return s_runtime; }

} // namespace tiny_cherno
