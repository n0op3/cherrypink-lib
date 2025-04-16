#pragma once

#include "event/event.hpp"

namespace tiny_cherno {

    class UpdateEvent : public Event {
        public:
            UpdateEvent() : Event(EventType::UPDATE_EVENT) {}
    };

}
