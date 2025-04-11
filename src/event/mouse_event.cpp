#include "mouse_event.hpp"
#include "event.hpp"

namespace tiny_cherno {

MouseMoveEvent::MouseMoveEvent(double x, double y)
    : Event(EventType::MouseEvent), x(x), y(y) {}

MouseButtonEvent::MouseButtonEvent(int button, int action)
    : Event(EventType::MouseEvent), button(button), action(action) {}

MouseScrollEvent::MouseScrollEvent(double xOffset, double yOffset)
    : Event(EventType::MouseEvent), xOffset(xOffset), yOffset(yOffset) {}

} // namespace tiny_cherno
