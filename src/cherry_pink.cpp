#include "cherry_pink.hpp"
#include "component/mesh_system.hpp"
#include "component/transform_component.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "event/render_event.hpp"
#include "event/update_event.hpp"
#include "profiling/profiler.hpp"
#include "rendering/opengl/opengl.hpp"
#include "rendering/renderer.hpp"
#include "rendering/window.hpp"
#include "scene/scene.hpp"
#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"
#include <chrono>
#include <memory>
#include <optional>

namespace cherrypink {

    static bool s_initialized = false;
    static bool s_shouldStop = false;
    static Window *s_window;
    static std::vector<Scene> s_scenes(1);
    static Scene *s_currentScene;
    static Renderer *s_renderer;
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

        #ifdef DEBUG
        #include "spdlog/common.h"
        spdlog::set_level(spdlog::level::debug);
        #endif // DEBUG

        spdlog::info("Initializing the CherryPink runtime!");

        profiler::Begin("init");

        glfwSetErrorCallback([](int errorCode, const char *description) { 
            spdlog::error("GLFW error {}: {}", errorCode, description); 
        });


        profiler::Begin("glfw init");

        if (!glfwInit()) {
            spdlog::critical("Failed to initialize GLFW");
            return GLFW_FAILED;
        }

        if (!windowParameters.resizable)
            glfwWindowHint(GLFW_RESIZABLE, false);

        s_window = new Window(windowParameters);
        s_renderer = new Renderer(new OpenGLContext(s_window));

        if (!s_renderer->Context()->Init()) {
            spdlog::error("Could not create the rendering context");
            return RENDERING_ERROR;
        }

        profiler::End("glfw init");

        profiler::Begin("rendering init");

        OpenGLShader vertex(GL_VERTEX_SHADER, R"(
                #version 330 core

                layout(location = 0) in vec3 aPos;

                uniform mat4 transform;
                uniform mat4 cameraView;
                uniform mat4 cameraProjection;

                void main() {
                    gl_Position = cameraProjection * cameraView * transform * vec4(aPos, 1.0);
                }
                )");

        OpenGLShader fragment(GL_FRAGMENT_SHADER, R"(
                #version 330 core

                out vec4 FragColor;

                void main() {
                    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
                }
                )");

        OpenGLShaderProgram *program = new OpenGLShaderProgram(&vertex, &fragment);

        s_renderer->SetProgram(program);

        profiler::End("rendering init");

        registerCallbacks();

        s_currentScene = &s_scenes.front();

        s_initialized = true;

        profiler::End("init");
        
        profiler::PrintResults();

        return NONE;
    }

    void update() {
        s_currentScene->componentRegistry.updateComponents(s_systems);
    }

    void shutdown() {
        profiler::Begin("shutdown");

        s_scenes.clear();
        s_renderer->Shutdown();
        s_eventDispatcher.Shutdown();
        s_systems.Shutdown();
        s_initialized = false;

        delete s_window;
        delete s_renderer;
        s_currentScene = nullptr;
        s_renderer = nullptr;

        glfwTerminate();

        profiler::End("shutdown");
        profiler::PrintResults();
    }

    bool Run() {
        if (!IsInitialized()) {
            spdlog::error("Trying to run, but the runtime was not initialized");
            return false;
        }

        spdlog::info("Entering the main loop...");
        auto lastTick = std::chrono::high_resolution_clock::now();
        auto lastRender = std::chrono::high_resolution_clock::now();
        while (!s_window->ShouldClose() && !s_shouldStop) {
            cherrypink::profiler::Begin();
            s_window->Update();

            auto now = std::chrono::high_resolution_clock::now();
            const long timeSinceLastTick = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick).count();
            const long timeSinceLastRender = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRender).count();
            const double secondInMs = 1000;

            if (timeSinceLastTick >= secondInMs / s_updateRate) {
                profiler::Begin("update");
                profiler::Begin("events");
                s_eventDispatcher.Dispatch(std::make_shared<UpdateEvent>());
                s_eventDispatcher.ProcessQueue();
                profiler::End("events");
                update();
                lastTick = std::chrono::high_resolution_clock::now();
                profiler::End("update");
            }

            if (timeSinceLastRender >= secondInMs / s_targetFPS) {
                profiler::Begin("render");
                double partialTicks = (double) timeSinceLastTick / s_targetFPS;
                s_renderer->UseCamera(&CurrentScene().camera);
                s_renderer->Prepare();

                for (auto &[uuid, meshComponent] : CurrentScene().componentRegistry.GetComponents<MeshComponent>()) {
                    if (CurrentScene().componentRegistry.GetComponent<TransformComponent>(uuid) != std::nullopt) {
                        const TransformComponent *transform = *CurrentScene().componentRegistry.GetComponent<TransformComponent>(uuid);
                        s_renderer->DrawMesh(*transform, meshComponent->mesh);
                    }
                }

                s_eventDispatcher.DispatchImmediately(std::make_shared<RenderEvent>(s_renderer->Context(), partialTicks));
                s_renderer->Finish();
                lastRender = std::chrono::high_resolution_clock::now();
                profiler::End("render");
            }

            profiler::PrintResults();
        }

        shutdown();
        return true;
    }

    void Stop() {
        s_shouldStop = true;
    }

    void SetUpdateRate(unsigned int updateRate) { if (updateRate >= 0) s_updateRate = updateRate; }

    void SetTargetFPS(unsigned int fps) { if (fps >= 0) s_targetFPS = fps; }

    int UpdateRate() { return s_updateRate; }

    int TargetFPS() { return s_targetFPS; }

    bool IsInitialized() { return s_initialized; }

    Window &GetWindow() { return *s_window; }

    EventDispatcher &Events() { return s_eventDispatcher; }

    Scene &CurrentScene() { return *s_currentScene; }

    SystemRegistry &Systems() { return s_systems; }

    Renderer &GetRenderer() { return *s_renderer; }

} // namespace cherrypink
