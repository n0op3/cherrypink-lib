#include "runtime.hpp"
#include "GLFW/glfw3.h"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "spdlog/spdlog.h"
#include <memory>

namespace tiny_cherno {

void TinyChernoRuntime::Run() {
    glfwWindowHint(GLFW_VISIBLE, true);

    spdlog::info("Entering the main loop...");
    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        eventDispatcher.ProcessQueue();
        update();
        glfwSwapBuffers(m_window);
    }
}

void TinyChernoRuntime::update() {
    CurrentScene()->componentRegistry.updateComponents(systems);
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

        m_scenes.push_back(Scene());
        m_currentScene = &m_scenes.front();
}

Scene *TinyChernoRuntime::CurrentScene() {
    return m_currentScene;
}

}
