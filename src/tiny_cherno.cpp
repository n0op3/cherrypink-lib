#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "glad/glad.h"

#include "rendering/window.hpp"
#include "tiny_cherno.hpp"

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

namespace tiny_cherno {

void error_callback(int error_code, const char *description) {
    spdlog::error("GLFW error {}: {}", error_code, description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    class KeyEvent e(key, action, mods);
    TinyChernoRuntime::GetRuntime()->event_dispatcher.Dispatch(std::make_shared<class KeyEvent>(key, action, mods));
}

void mouse_move_callback(GLFWwindow *window, double x, double y) {
    MouseMoveEvent e(x, y);
    TinyChernoRuntime::GetRuntime()->event_dispatcher.Dispatch(std::make_shared<MouseMoveEvent>(x, y));
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
    MouseButtonEvent e(button, action);
    TinyChernoRuntime::GetRuntime()->event_dispatcher.Dispatch(std::make_shared<MouseButtonEvent>(button, action));
}

static TinyChernoRuntime *s_runtime = nullptr;

InitializationError init(WindowParameters &window_parameters) {
    if (s_runtime) {
        return NONE;
    }

    spdlog::info("Initializing the TinyCherno runtime!");
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
        glfwPollEvents();
        event_dispatcher.ProcessQueue();
        glfwSwapBuffers(m_window);
    }
}

TinyChernoRuntime::TinyChernoRuntime(GLFWwindow *window)
    : m_window(window), event_dispatcher(EventDispatcher()) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

TinyChernoRuntime *TinyChernoRuntime::GetRuntime() { return s_runtime; }

} // namespace tiny_cherno
