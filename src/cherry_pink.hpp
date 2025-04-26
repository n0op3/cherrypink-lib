#pragma once

#include "event/event.hpp"
#include "rendering/renderer.hpp"
#include "rendering/window.hpp"
#include "scene/scene.hpp"

struct GLFWwindow;

namespace cherrypink {

    #define ASSERTION_FAILED_EXIT_CODE 712

    #ifdef DEBUG
        #define TC_ASSERT(expr, message) \
            if (!(expr)) { \
                spdlog::error("ASSERTION FAILED line {} in {}: {}", __LINE__, __FILE__, message); \
            }
    #else
        #define TC_ASSERT(expr, message) \
            /* removed */
    #endif // DEBUG

    #define TC_CRITICAL_ASSERT(expr, message) \
        if (!(expr)) { \
            spdlog::critical("ASSERTION FAILED line {} in {}: {}", __LINE__, __FILE__, message); \
            exit(ASSERTION_FAILED_EXIT_CODE); \
        }

    enum InitializationError {
        NONE,
        GLFW_FAILED,
        GLAD_FAILED,
        RENDERING_ERROR
    };

    InitializationError Init(WindowParameters windowParameters);
    bool IsInitialized();
    bool Run();
    void Stop();
    int UpdateRate();
    void SetUpdateRate(unsigned int updateRate);
    int TargetFPS();
    void SetTargetFPS(unsigned int fps);
    EventDispatcher &Events();
    Window &GetWindow();
    Scene &CurrentScene();
    SystemRegistry &Systems();
    Renderer &GetRenderer();

} // namespace cherrypink
