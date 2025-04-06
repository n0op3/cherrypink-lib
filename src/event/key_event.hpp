#pragma once

#include "event.hpp"
namespace tiny_cherno {

class KeyEvent : Event {
  public:
    KeyEvent(int key, int action, int modifiers);
    const int key;
    const int action;
    const int modifiers;
};

} // namespace tiny_cherno
