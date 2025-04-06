#include "event.hpp"

namespace tiny_cherno {

Event::Event(EventType type) : m_type(type) {};

EventType Event::getType() { return m_type; }

} // namespace tiny_cherno
