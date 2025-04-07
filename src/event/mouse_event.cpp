#include "mouse_event.hpp"
#include "event.hpp"

namespace tiny_cherno {

MouseMoveEvent::MouseMoveEvent(double x, double y)
    : x(x), y(y), Event(EventType::MouseEvent) {}

MouseButtonEvent::MouseButtonEvent(int button, int action)
    : button(button), action(action), Event(EventType::MouseEvent) {}

} // namespace tiny_cherno
