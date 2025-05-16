#include "cherry_pink.hpp"

#include <algorithm>
#include <chrono>
#include <memory>
#include <thread>

#include "GLFW/glfw3.h"
#include "assets/resource_manager.hpp"
#include "component/render_system.hpp"
#include "component/transform_component.hpp"
#include "component/transform_system.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "event/render_event.hpp"
#include "event/update_event.hpp"
#include "profiling/profiler.hpp"
#include "rendering/opengl/opengl.hpp"
#include "rendering/renderer.hpp"
#include "rendering/window.hpp"
#include "scene/scene.hpp"
#include "spdlog/spdlog.h"

namespace cherrypink {

    static bool s_initialized = false;
    static bool s_shouldStop = false;
    static Window *s_window;
    static std::vector<Scene> s_scenes(1);
    static Scene *s_currentScene;
    static Renderer *s_renderer;
    static EventDispatcher s_eventDispatcher;
    static SystemRegistry s_systems;
    static ResourceManager s_resourceManager("assets");
    static int s_targetFPS = 60;
    static int s_updateRate = 20;
    static int s_ticks = 0;
    static int s_frames = 0;

    void registerCallbacks() {
        glfwSetKeyCallback(s_window->Handle(), [](GLFWwindow *window, int key,
                    int scancode, int action,
                    int mods) {
                s_eventDispatcher.Dispatch(std::make_unique<KeyEvent>(key, action, mods));
                });
        glfwSetCursorPosCallback(
                s_window->Handle(), [](GLFWwindow *window, double x, double y) {
                s_eventDispatcher.Dispatch(std::make_unique<MouseMoveEvent>(x, y));
                });
        glfwSetMouseButtonCallback(
                s_window->Handle(),
                [](GLFWwindow *window, int button, int action, int mods) {
                s_eventDispatcher.Dispatch(
                        std::make_unique<MouseButtonEvent>(button, action));
                });
        glfwSetScrollCallback(s_window->Handle(), [](GLFWwindow *window,
                    double xOffset, double yOffset) {
                s_eventDispatcher.Dispatch(
                        std::make_unique<MouseScrollEvent>(xOffset, yOffset));
                });
    }

    void registerBaseSystems() {
        Systems().RegisterUpdateSystem<Transform>(
                std::make_unique<TransformSystem>());
        Systems().RegisterRenderSystem<Mesh>(std::make_unique<RenderingSystem>());
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
                layout(location = 1) in vec2 aTexCoord;

                uniform mat4 transform;
                uniform mat4 cameraView;
                uniform mat4 cameraProjection;

                out vec2 FragTexCoord;

                void main() {
                    gl_Position = cameraProjection * cameraView * transform * vec4(aPos, 1.0);
                }
                )");

        OpenGLShader fragment(GL_FRAGMENT_SHADER, R"(
                #version 330 core

                struct Material {
                    vec4 color;
                    bool isTextured;
                    sampler2D texture;
                };

                in vec2 FragTexCoord;

                uniform Material material;

                out vec4 FragColor;

                void main() {
                    FragColor = material.color;

                    if (material.isTextured) {
                        FragColor = FragColor * texture(material.texture, FragTexCoord);
                    }
                }
                )");

        OpenGLShaderProgram *program = new OpenGLShaderProgram(&vertex, &fragment);

        s_renderer->SetProgram(program);

        profiler::End("rendering init");

        registerCallbacks();
        registerBaseSystems();

        s_currentScene = &s_scenes.front();

        s_initialized = true;

        profiler::End("init");

        profiler::PrintResults();

        return NONE;
    }

    void update(double deltaTime) {
        s_currentScene->componentRegistry.updateComponents(s_systems, deltaTime);
        s_ticks++;
    }

    void render(double partialTicks) {
        s_renderer->UseCamera(&CurrentScene().camera);
        s_renderer->Prepare();

        s_eventDispatcher.DispatchImmediately(
                std::make_unique<RenderEvent>(s_renderer->Context(), partialTicks));
        s_currentScene->componentRegistry.updateRenderComponents(s_systems,
                partialTicks);
        s_renderer->Finish();
        s_frames++;
    }

    void shutdown() {
        profiler::Begin("shutdown");

        s_scenes.clear();
        s_renderer->Shutdown();
        s_eventDispatcher.Shutdown();
        s_resourceManager.Shutdown();
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
        bool slept = false;
        while (!s_window->ShouldClose() && !s_shouldStop) {
            cherrypink::profiler::Begin();
            s_window->Update();

            auto now = std::chrono::high_resolution_clock::now();
            const long timeSinceLastTick =
                std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTick)
                .count();
            const long timeSinceLastRender =
                std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRender)
                .count();
            const double secondInMs = 1000;

            if (s_updateRate != 0 && timeSinceLastTick >= secondInMs / s_updateRate) {
                profiler::Begin("update");
                double deltaTime = timeSinceLastTick / (secondInMs / s_updateRate);
                profiler::Begin("events");
                s_eventDispatcher.Dispatch(std::make_unique<UpdateEvent>(deltaTime));
                s_eventDispatcher.ProcessQueue();
                profiler::End("events");

                update(deltaTime);

                lastTick = std::chrono::high_resolution_clock::now();
                profiler::End("update");

                slept = false;
            }

            if (timeSinceLastRender >= secondInMs / s_targetFPS) {
                profiler::Begin("render");
                double partialTicks =
                    std::min(1.0, (double)timeSinceLastTick / s_targetFPS);

                render(partialTicks);

                lastRender = std::chrono::high_resolution_clock::now();
                profiler::End("render");

                slept = false;
            }

            if (!slept) {
                std::this_thread::sleep_for(std::chrono::duration<double>(
                            1.0 / std::max(s_updateRate, s_targetFPS) - 20.0));
                slept = true;
            }

            profiler::PrintResults();
        }

        shutdown();
        return true;
    }

    void Stop() { s_shouldStop = true; }

    uint64_t Ticks() { return s_ticks; }

    uint64_t Frames() { return s_frames; }

    void SetUpdateRate(unsigned int updateRate) {
        if (updateRate >= 0)
            s_updateRate = updateRate;
    }

    void SetTargetFPS(unsigned int fps) {
        if (fps >= 0)
            s_targetFPS = fps;
    }

    int UpdateRate() { return s_updateRate; }

    int TargetFPS() { return s_targetFPS; }

    bool IsInitialized() { return s_initialized; }

    Window &GetWindow() { return *s_window; }

    EventDispatcher &Events() { return s_eventDispatcher; }

    Scene &CurrentScene() { return *s_currentScene; }

    SystemRegistry &Systems() { return s_systems; }

    Renderer &GetRenderer() { return *s_renderer; }

    ResourceManager &GetResourceManager() { return s_resourceManager; }

} // namespace cherrypink
