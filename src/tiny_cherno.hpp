#pragma once
#include "rendering/window.hpp"

namespace tiny_cherno {

enum InitializationError {
    NONE,
    GLFW_FAILED,
    GLAD_FAILED,
};

InitializationError init(WindowParameters &window_parameters);

} // namespace tiny_cherno
