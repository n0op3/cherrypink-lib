#pragma once
#include "rendering/window.hpp"

class GLFWwindow;

namespace tiny_cherno {

enum InitializationError {
    NONE,
    GLFW_FAILED,
    GLAD_FAILED,
};

InitializationError init(WindowParameters &window_parameters);
void run();

class TinyChernoRuntime {
  public:
    TinyChernoRuntime(GLFWwindow *window);
    void Run();
    static TinyChernoRuntime *GetRuntime();

  private:
    GLFWwindow *m_window;
};

} // namespace tiny_cherno
