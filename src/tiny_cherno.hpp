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
    };

    InitializationError Init(WindowParameters windowParameters);
    bool IsInitialized();
    bool Run();
    EventDispatcher& Events();
    Scene& CurrentScene();
    SystemRegistry& Systems();

} // namespace tiny_cherno
