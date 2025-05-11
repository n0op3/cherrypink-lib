#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <typeindex>
#include <utility>
#include <vector>

namespace cherrypink {

class Event {
public:
    virtual ~Event() = default;
};

template <typename T>
using Listener = std::function<void(const T &event)>;
using EventListener = Listener<Event>;

class EventDispatcher {
public:
    EventDispatcher() = default;
    template <typename T>
    void Dispatch(std::unique_ptr<T> e) {
        m_events.push(std::move(e));
    }

    template <typename T>
    void DispatchImmediately(std::unique_ptr<T> e) {
        processEvent<T>(*e);
    }

    template <typename T>
    void RegisterListener(Listener<T> listener) {
        m_listeners[std::type_index(typeid(T))].push_back(
            [listener](const Event &event) {
                listener(static_cast<const T &>(event));
            });
    }

    void ProcessQueue();
    void ClearQueue();
    bool IsDone() const;
    void Shutdown();

private:
    std::optional<Event> nextEvent();
    template <typename T>
    void processEvent(const T &event) {
        auto typeIndex = std::type_index(typeid(T));
        auto it = m_listeners.find(typeIndex);
        if (it != m_listeners.end()) {
            for (Listener<Event> listener : it->second) {
                listener(event);
            }
        }
    }

private:
    std::queue<std::unique_ptr<Event>> m_events;
    std::map<std::type_index, std::vector<EventListener>> m_listeners;
};

}  // namespace cherrypink
