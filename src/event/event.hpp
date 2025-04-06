#pragma once

namespace tiny_cherno {

enum EventType { WindowEvent, KeyEvent, MouseEvent };

class Event {
  public:
    Event(EventType type);
    EventType getType();

  private:
    const EventType m_type;
};

} // namespace tiny_cherno
