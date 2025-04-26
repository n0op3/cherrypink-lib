#pragma once

#include "event/event.hpp"

namespace cherrypink {

    class UpdateEvent : public Event {
        public:
            UpdateEvent() : Event(EventType::UPDATE_EVENT) {}
    };

}
