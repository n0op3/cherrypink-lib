#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "glad/glad.h"

#include "rendering/window.hpp"
#include "tiny_cherno.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include <memory>

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

void TinyChernoRuntime::Run() {
    glfwWindowHint(GLFW_VISIBLE, true);

    spdlog::info("Entering the main loop...");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        eventDispatcher.ProcessQueue();
        glfwSwapBuffers(m_window);
    }
}

TinyChernoRuntime::TinyChernoRuntime(GLFWwindow *window)
    : m_window(window) {
        glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods){
            TinyChernoRuntime::GetRuntime()->eventDispatcher.Dispatch(std::make_shared<class KeyEvent>(key, action, mods));
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y){
            TinyChernoRuntime::GetRuntime()->eventDispatcher.Dispatch(std::make_shared<MouseMoveEvent>(x, y));
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods){
            TinyChernoRuntime::GetRuntime()->eventDispatcher.Dispatch(std::make_shared<MouseButtonEvent>(button, action));
        });
        glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset){
            TinyChernoRuntime::GetRuntime()->eventDispatcher.Dispatch(std::make_shared<MouseScrollEvent>(xOffset, yOffset));
        });
}

TinyChernoRuntime *TinyChernoRuntime::GetRuntime() { return s_runtime; }

} // namespace tiny_cherno
