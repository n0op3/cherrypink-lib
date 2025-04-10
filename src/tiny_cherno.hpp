#pragma once

#include "event/event.hpp"
#include "rendering/window.hpp"

struct GLFWwindow;

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

    EventDispatcher event_dispatcher;

  private:
    GLFWwindow *m_window;
};

} // namespace tiny_cherno
