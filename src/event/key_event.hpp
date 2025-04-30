#pragma once

#include "event/event.hpp"

namespace cherrypink {

    struct KeyEvent : public Event {
        KeyEvent(int key, int action, int modifiers)
            : key(key), action(action), modifiers(modifiers) {}

        const int key;
        const int action;
        const int modifiers;
    };

} // namespace cherrypink
