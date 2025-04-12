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
void run();

} // namespace tiny_cherno
