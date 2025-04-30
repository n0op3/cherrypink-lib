#pragma once

#include "event/event.hpp"

namespace cherrypink {

    struct UpdateEvent : public Event {
        explicit UpdateEvent(double deltaTime) : deltaTime(deltaTime) {}
        
        const double deltaTime;
    };

}
