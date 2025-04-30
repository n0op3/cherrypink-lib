#pragma once

#include "event/event.hpp"
#include "rendering/context.hpp"

namespace cherrypink {
    
    struct RenderEvent : public Event {
            RenderEvent(const RenderingContext *context, double partialTicks)
                : context(context), partialTicks(partialTicks) {}

            const RenderingContext *context;
            const double partialTicks;
    };

}
