#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

namespace tiny_cherno {

enum EventType { WindowEvent, KeyEvent, MouseEvent };

class Event {
  public:
    Event(EventType type);
    const EventType type;
};

using EventListener = std::function<bool(Event &event)>;
class EventDispatcher {
  public:
    EventDispatcher();
    void Dispatch(std::shared_ptr<Event> e);
    void ProcessQueue();
    void RegisterListener(EventType type, EventListener listener);
    void ClearQueue();
    bool IsDone();
    void Shutdown();

  private:
    std::optional<std::shared_ptr<Event>> nextEvent();

  private:
    std::queue<std::shared_ptr<Event>> m_events;
    std::map<EventType, std::vector<EventListener>> m_listeners;
};

} // namespace tiny_cherno
