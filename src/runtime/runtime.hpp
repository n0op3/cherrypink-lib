#pragma once

#include "component/component.hpp"
#include "event/event.hpp"
#include "scene/scene.hpp"
#include <vector>

struct GLFWwindow;

namespace tiny_cherno {

class TinyChernoRuntime {
  public:
    TinyChernoRuntime(GLFWwindow *window);
    static TinyChernoRuntime *GetRuntime();
    void Run();
    Scene* CurrentScene();

  public:
    EventDispatcher eventDispatcher;
    SystemRegistry systems;

  private:
    void update();

  private:
    GLFWwindow *m_window;
    std::vector<Scene> m_scenes;
    Scene *m_currentScene;
};

}
