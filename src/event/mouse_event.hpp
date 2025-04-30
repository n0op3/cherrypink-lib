#pragma once

#include "event/event.hpp"

namespace cherrypink {

    struct MouseMoveEvent : public Event {
        MouseMoveEvent(double x, double y) : x(x), y(y) {}

        const double x;
        const double y;
    };

    struct MouseButtonEvent : public Event {
        MouseButtonEvent(double button, int action)
            : button(button), action(action) {}

        const double button;
        const int action;
    };

    struct MouseScrollEvent : public Event {
        MouseScrollEvent(double xOffset, double yOffset)
            : xOffset(xOffset), yOffset(yOffset) {}

        const double xOffset;
        const double yOffset;
    };

} // namespace cherrypink
