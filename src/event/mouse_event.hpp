#pragma once

#include "event.hpp"

namespace tiny_cherno {

class MouseMoveEvent : public Event {
  public:
    MouseMoveEvent(double x, double y);

  public:
    const double x;
    const double y;
};

class MouseButtonEvent : public Event {
  public:
    MouseButtonEvent(int mouse_button, int action);

  public:
    const double button;
    const int action;
};

} // namespace tiny_cherno
