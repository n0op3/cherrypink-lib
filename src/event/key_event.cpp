#include "key_event.hpp"
#include "event.hpp"

namespace cherrypink {

KeyEvent::KeyEvent(int key, int action, int modifiers)
    : Event(EventType::KEY_EVENT), key(key), action(action),
      modifiers(modifiers) {}

}; // namespace cherrypink
