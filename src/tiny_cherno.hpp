#pragma once

#include "rendering/window.hpp"

struct GLFWwindow;

namespace tiny_cherno {

enum InitializationError {
    NONE,
    GLFW_FAILED,
    GLAD_FAILED,
};

InitializationError init(WindowParameters &windowParameters);
bool is_initialized();
bool run();

} // namespace tiny_cherno
