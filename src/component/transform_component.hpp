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

        TransformComponent() {
            position = glm::vec3();
            rotation = glm::vec3();
            scale = glm::vec3(1);
        }

        glm::mat4 ModelMatrix() const {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::scale(model, scale);

            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

            model = glm::translate(model, position);

            return model;
        }
    };

}
