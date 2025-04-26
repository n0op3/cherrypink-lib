#pragma once

#include "event.hpp"

namespace cherrypink {

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

class MouseScrollEvent : public Event {
    public:
        MouseScrollEvent(double xOffset, double yOffset);

    public:
        const double xOffset;
        const double yOffset;
};

} // namespace cherrypink
