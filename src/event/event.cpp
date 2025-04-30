#include "event.hpp"
#include <cstdio>
#include <memory>
#include <optional>
#include <queue>
#include <utility>

namespace cherrypink {

    void EventDispatcher::ProcessQueue() {
        while (!IsDone()) {
            auto event = nextEvent();
            processEvent(*event);
        }
    }

    void EventDispatcher::ClearQueue() {
        while (!IsDone())
            m_events.pop();
    }

    bool EventDispatcher::IsDone() const {
        return m_events.empty();
    }

    void EventDispatcher::Shutdown() {
        ClearQueue();
        m_listeners.clear();
    }

    std::optional<Event> EventDispatcher::nextEvent() {
        if (IsDone())
            return std::nullopt;

        auto event = std::move(*m_events.front());
        m_events.pop();

        return event;
    }

} // namespace cherrypink
