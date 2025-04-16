#include "tiny_cherno.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "rendering/window.hpp"
#include "rendering/opengl/context.cpp"
#include "scene/scene.hpp"
#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"

namespace tiny_cherno {

    static bool s_initialized = false;
    static Window *s_window;
    static std::vector<Scene> s_scenes(1);
    static Scene *s_currentScene;
    static EventDispatcher s_eventDispatcher;
    static SystemRegistry s_systems;

    void registerCallbacks() {
        glfwSetKeyCallback(s_window->Handle(),
            [] (GLFWwindow *window, int key, int scancode, int action, int mods) {
            s_eventDispatcher.Dispatch(std::make_shared<class KeyEvent>(key, action, mods));
        });
        glfwSetCursorPosCallback(s_window->Handle(),
            [] (GLFWwindow *window, double x, double y) {
            s_eventDispatcher.Dispatch(std::make_shared<MouseMoveEvent>(x, y));
        });
        glfwSetMouseButtonCallback(s_window->Handle(),
            [] (GLFWwindow *window, int button, int action, int mods) {
            s_eventDispatcher.Dispatch(std::make_shared<MouseButtonEvent>(button, action));
        });
        glfwSetScrollCallback(s_window->Handle(),
            [] (GLFWwindow *window, double xOffset, double yOffset) {
            s_eventDispatcher.Dispatch(std::make_shared<MouseScrollEvent>(xOffset, yOffset));
        });
    }

    InitializationError Init(WindowParameters windowParameters) {
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

        s_window = new Window(windowParameters);
        s_window->SetRenderingContext(new OpenGLContext(s_window->Handle()));
        if (!s_window->Context()->Init()) {
            spdlog::error("Could not create the rendering context");
            return RENDERING_ERROR;
        }

        registerCallbacks();

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

        spdlog::info("Entering the main loop...");
        while (!s_window->ShouldClose()) {
            s_eventDispatcher.ProcessQueue();
            update();
            s_window->Update();
        }

        Shutdown();
        return true;
    }

    void Shutdown() {
        delete s_window;
        s_scenes.clear();
        s_currentScene = nullptr;
        s_eventDispatcher.Shutdown();
        s_systems.Shutdown();
        s_initialized = false;
    }

    bool IsInitialized() { return s_initialized; }

    Window *GetWindow() { return s_window; }

    EventDispatcher& Events() { return s_eventDispatcher; }

    Scene* CurrentScene() { return s_currentScene; }

    SystemRegistry& Systems() { return s_systems; }

} // namespace tiny_cherno
