#pragma once

#include "event/event.hpp"

namespace tiny_cherno {
    
    class RenderEvent : public Event {
        public:
            RenderEvent(double deltaTime) : Event(EventType::RENDER_EVENT), deltaTime(deltaTime) {}
            const double deltaTime;
    };

}
