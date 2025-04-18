#include "event.hpp"
#include <cstdio>
#include <memory>
#include <optional>
#include <queue>
#include <vector>

namespace tiny_cherno {

Event::Event(EventType type) : type(type) {};

EventDispatcher::EventDispatcher()
    : m_events(std::queue<std::shared_ptr<Event>>()) {}

std::optional<std::shared_ptr<Event>> EventDispatcher::nextEvent() {
    if (m_events.empty())
        return std::nullopt;

    std::shared_ptr<Event> e = std::move(m_events.front());
    m_events.pop();
    return e;
}

void EventDispatcher::processEvent(std::shared_ptr<Event> e) {
    if (m_listeners.find(e->type) == m_listeners.end())
        return;

    for (EventListener listener : m_listeners[e->type])
        listener(*e);
}

void EventDispatcher::Dispatch(std::shared_ptr<Event> e) {
    m_events.push(e);
}

void EventDispatcher::DispatchImmediately(std::shared_ptr<Event> e) {
    processEvent(e);
}

void EventDispatcher::ProcessQueue() {
    while (!IsDone()) {
        std::optional<std::shared_ptr<Event>> e = nextEvent();
        if (!e.has_value())
            continue;

        processEvent(*e);
    }
}

void EventDispatcher::RegisterListener(EventType type, EventListener listener) {
    auto it = m_listeners.find(type);
    if (it == m_listeners.end())
        m_listeners[type] = std::vector<EventListener>();

    m_listeners[type].push_back(listener);
}

void EventDispatcher::ClearQueue() {
    while (!IsDone())
        m_events.pop();
}

bool EventDispatcher::IsDone() { return m_events.empty(); }

void EventDispatcher::Shutdown() {
    m_listeners.clear();
    while (!m_events.empty())
        m_events.pop();
}

} // namespace tiny_cherno
