#pragma once

#include "event/event.hpp"

struct GLFWwindow;

namespace tiny_cherno {

class TinyChernoRuntime {
  public:
    TinyChernoRuntime(GLFWwindow *window);
    void Run();
    static TinyChernoRuntime *GetRuntime();

  public:
    EventDispatcher eventDispatcher;

  private:
    GLFWwindow *m_window;
};

}
