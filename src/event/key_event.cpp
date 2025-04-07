#include "key_event.hpp"
#include "event.hpp"

namespace tiny_cherno {

KeyEvent::KeyEvent(int key, int action, int modifiers)
    : Event(EventType::KeyEvent), key(key), action(action),
      modifiers(modifiers) {}

}; // namespace tiny_cherno
