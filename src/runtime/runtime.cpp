#include "runtime.hpp"
#include "GLFW/glfw3.h"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "spdlog/spdlog.h"

namespace tiny_cherno {

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

}
