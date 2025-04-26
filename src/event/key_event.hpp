#pragma once

#include "event.hpp"
namespace cherrypink {

class KeyEvent : public Event {
  public:
    KeyEvent(int key, int action, int modifiers);

  public:
    const int key;
    const int action;
    const int modifiers;
};

} // namespace cherrypink
