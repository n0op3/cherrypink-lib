#pragma once

#include "event/event.hpp"
#include "rendering/context.hpp"

namespace tiny_cherno {
    
    class RenderEvent : public Event {
        public:
            RenderEvent(RenderingContext *context, double partialTicks) : Event(EventType::RENDER_EVENT), context(context), partialTicks(partialTicks) {}
            const RenderingContext *context;
            const double partialTicks;
    };

}
