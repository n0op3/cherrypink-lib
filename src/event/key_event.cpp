#include "key_event.hpp"
#include "event.hpp"

namespace tiny_cherno {

KeyEvent::KeyEvent(int key, int action, int modifiers)
    : key(key), action(action), modifiers(modifiers),
      Event(EventType::KeyEvent) {}

}; // namespace tiny_cherno
