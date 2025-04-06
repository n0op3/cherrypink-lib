#include "mouse_event.hpp"
#include "event.hpp"

namespace tiny_cherno {

MouseEvent::MouseEvent(int x, int y)
    : x(x), y(y), Event(EventType::MouseEvent) {}

MouseMoveEvent::MouseMoveEvent(int x, int y, int prev_x, int prev_y)
    : prevX(prev_x), prevY(prev_y), MouseEvent(x, y) {}
const int MouseMoveEvent::deltaX() { return x - prevX; }
const int MouseMoveEvent::deltaY() { return y - prevY; }

} // namespace tiny_cherno
