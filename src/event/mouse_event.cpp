#include "mouse_event.hpp"
#include "event.hpp"

namespace cherrypink {

MouseMoveEvent::MouseMoveEvent(double x, double y)
    : Event(EventType::MOUSE_EVENT), x(x), y(y) {}

MouseButtonEvent::MouseButtonEvent(int button, int action)
    : Event(EventType::MOUSE_EVENT), button(button), action(action) {}

MouseScrollEvent::MouseScrollEvent(double xOffset, double yOffset)
    : Event(EventType::MOUSE_EVENT), xOffset(xOffset), yOffset(yOffset) {}

} // namespace cherrypink
