#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace cherrypink {

    struct TransformComponent {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 prevPosition;
        glm::vec3 prevRotation;
        glm::vec3 prevScale;

        TransformComponent() {
            position = glm::vec3();
            rotation = glm::vec3();
            scale = glm::vec3(1);
        }

        glm::mat4 ModelMatrix(float partialTicks) const {
            glm::mat4 model = glm::mat4(1.0f);

            glm::vec3 partialPosition = prevPosition + (position - prevPosition) * partialTicks;
            glm::vec3 partialRotation = prevRotation + (rotation - prevRotation) * partialTicks;
            glm::vec3 partialScale = prevScale + (scale - prevScale) * partialTicks;

            model = glm::scale(model, partialScale);

            model = glm::rotate(model, glm::radians(partialRotation.x), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(partialRotation.y), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(partialRotation.z), glm::vec3(0, 0, 1));

            model = glm::translate(model, partialPosition);

            return model;
        }
    };

}
