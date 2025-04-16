#pragma once

#include "event/event.hpp"
#include "rendering/window.hpp"
#include "scene/scene.hpp"

struct GLFWwindow;

namespace tiny_cherno {

    enum InitializationError {
        NONE,
        GLFW_FAILED,
        GLAD_FAILED,
        RENDERING_ERROR
    };

    InitializationError Init(WindowParameters windowParameters);
    bool IsInitialized();
    bool Run();
    void Shutdown();
    int UpdateRate();
    void SetUpdateRate(unsigned int updateRate);
    int TargetFPS();
    void SetTargetFPS(unsigned int fps);
    EventDispatcher &Events();
    Window *GetWindow();
    Scene *CurrentScene();
    SystemRegistry &Systems();

} // namespace tiny_cherno
