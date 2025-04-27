#pragma once

#include "component/system.hpp"
#include "component/transform_component.hpp"

namespace cherrypink {

    class TransformSystem : public System<TransformComponent> {
        void ProcessComponent(const UUID &entityUuid, TransformComponent &transform) override {
            transform.prevPosition = transform.position;
            transform.prevRotation = transform.rotation;
            transform.prevScale = transform.scale;
        }
    };

}
