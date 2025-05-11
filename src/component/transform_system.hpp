#pragma once

#include "component/system.hpp"
#include "component/transform_component.hpp"

namespace cherrypink {

class TransformSystem : public UpdateSystem<Transform> {
    void ProcessComponent(const UUID &entityUuid, Transform &transform,
                          double deltaTime) override {
        transform.prevPosition = transform.position;
        transform.prevRotation = transform.rotation;
        transform.prevScale = transform.scale;
    }
};

}  // namespace cherrypink
