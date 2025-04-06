#pragma once

#include "event.hpp"

namespace tiny_cherno {

class MouseEvent : Event {
  public:
    const int x;
    const int y;

  protected:
    MouseEvent(int x, int y);
};

class MouseMoveEvent : MouseEvent {
  public:
    MouseMoveEvent(int x, int y, int prev_x, int prev_y);
    const int deltaX();
    const int deltaY();
    const int prevX;
    const int prevY;
};

class MouseButtonEvent : MouseEvent {
  public:
    MouseButtonEvent(int x, int y, int mouse_button, int action);
    const int mouseButton;
};

} // namespace tiny_cherno
