#include "tiny_cherno.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "event/render_event.hpp"
#include "event/update_event.hpp"
#include "rendering/window.hpp"
#include "rendering/opengl/context.cpp"
#include "scene/scene.hpp"
#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"
#include <chrono>
#include <memory>

namespace tiny_cherno {

    static bool s_initialized = false;
    static Window *s_window;
    static std::vector<Scene> s_scenes(1);
    static Scene *s_currentScene;
    static EventDispatcher s_eventDispatcher;
    static SystemRegistry s_systems;
    static int s_targetFPS = 60;
    static int s_updateRate = 20;

    void registerCallbacks() {
        glfwSetKeyCallback(s_window->Handle(),
            [] (GLFWwindow *window, int key, int scancode, int action, int mods) {
            s_eventDispatcher.Dispatch(std::make_shared<KeyEvent>(key, action, mods));
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
        auto lastTick = std::chrono::high_resolution_clock::now();
        auto lastRender = std::chrono::high_resolution_clock::now();
        while (!s_window->ShouldClose()) {
            auto now = std::chrono::high_resolution_clock::now();
            const long timeSinceLastTick = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count();
            const long timeSinceLastRender = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRender).count();
            const double secondInMs = 1000;

            if (timeSinceLastTick >= secondInMs / s_updateRate) {
                s_eventDispatcher.Dispatch(std::make_shared<UpdateEvent>());
                s_eventDispatcher.ProcessQueue();
                update();
                lastTick = std::chrono::high_resolution_clock::now();
            }

            if (timeSinceLastRender >= secondInMs / s_targetFPS) {
                double deltaTime = (double) timeSinceLastTick / s_targetFPS;
                s_window->Context()->Clear();
                s_eventDispatcher.Dispatch(std::make_shared<RenderEvent>(s_window->Context(), deltaTime));
                s_window->Update();
                lastRender = std::chrono::high_resolution_clock::now();
            }
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

    void SetUpdateRate(unsigned int updateRate) { s_updateRate = updateRate; }

    void SetTargetFPS(unsigned int fps) { s_targetFPS = fps; }

    int UpdateRate() { return s_updateRate; }

    int TargetFPS() { return s_targetFPS; }

    bool IsInitialized() { return s_initialized; }

    Window *GetWindow() { return s_window; }

    EventDispatcher& Events() { return s_eventDispatcher; }

    Scene* CurrentScene() { return s_currentScene; }

    SystemRegistry& Systems() { return s_systems; }

} // namespace tiny_cherno
