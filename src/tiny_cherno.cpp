#include "glad/glad.h"

#include "rendering/window.hpp"
#include "tiny_cherno.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "GLFW/glfw3.h"
#include "scene/scene.hpp"
#include "spdlog/spdlog.h"

namespace tiny_cherno {

    static bool s_initialized = false;
    static GLFWwindow *s_window;
    static std::vector<Scene> s_scenes(1);
    static Scene *s_currentScene;
    static EventDispatcher s_eventDispatcher;
    static SystemRegistry s_systems;

    InitializationError Init(WindowParameters &windowParameters) {
        if (s_initialized)
            return NONE;

        spdlog::info("Initializing the TinyCherno runtime!");
        glfwSetErrorCallback([](int errorCode, const char *description) { 
            spdlog::error("GLFW error {}: {}", errorCode, description); 
        });

        if (!glfwInit()) {
            spdlog::critical("Failed to initialize GLFW");
            return GLFW_FAILED;
        }

        if (!windowParameters.resizable)
            glfwWindowHint(GLFW_RESIZABLE, false);

        s_window = glfwCreateWindow(
                windowParameters.width, windowParameters.height, windowParameters.title,
                NULL, NULL);

        glfwMakeContextCurrent(s_window);

        if (!gladLoadGL()) {
            spdlog::critical("Failed to load OpenGL symbols");
            return GLAD_FAILED;
        }

        glfwSetKeyCallback(s_window,
            [] (GLFWwindow *window, int key, int scancode, int action, int mods) {
            s_eventDispatcher.Dispatch(std::make_shared<class KeyEvent>(key, action, mods));
        });
        glfwSetCursorPosCallback(s_window,
            [] (GLFWwindow *window, double x, double y) {
            s_eventDispatcher.Dispatch(std::make_shared<MouseMoveEvent>(x, y));
        });
        glfwSetMouseButtonCallback(s_window,
            [] (GLFWwindow *window, int button, int action, int mods) {
            s_eventDispatcher.Dispatch(std::make_shared<MouseButtonEvent>(button, action));
        });
        glfwSetScrollCallback(s_window,
            [] (GLFWwindow *window, double xOffset, double yOffset) {
            s_eventDispatcher.Dispatch(std::make_shared<MouseScrollEvent>(xOffset, yOffset));
        });

        s_currentScene = &s_scenes.front();

        s_initialized = true;
        return NONE;
    }

    void update() {
        s_currentScene->componentRegistry.updateComponents(s_systems);
    }

    bool Run() {
        if (!IsInitialized()) {
            spdlog::error("Trying to run, but the runtime was not initialized");
            return false;
        }

        glfwWindowHint(GLFW_VISIBLE, true);

        spdlog::info("Entering the main loop...");
        while (!glfwWindowShouldClose(s_window)) {
            glfwPollEvents();
            s_eventDispatcher.ProcessQueue();
            update();
            glfwSwapBuffers(s_window);
        }

        return true;
    }

    bool IsInitialized() { return s_initialized; }

    EventDispatcher& Events() { return s_eventDispatcher; }

    Scene& CurrentScene() { return *s_currentScene; }

    SystemRegistry& Systems() { return s_systems; }

} // namespace tiny_cherno
