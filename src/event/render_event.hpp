#pragma once

#include "event/event.hpp"
#include "rendering/context.hpp"

namespace tiny_cherno {
    
    class RenderEvent : public Event {
        public:
            RenderEvent(RenderingContext *context, double deltaTime) : Event(EventType::RENDER_EVENT), context(context), deltaTime(deltaTime) {}
            const RenderingContext *context;
            const double deltaTime;
    };

}
