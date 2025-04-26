#pragma once

#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/vector_float3.hpp>

namespace tiny_cherno {

    class Camera {
        public:
            Camera();
            glm::vec3 Direction() const;
            glm::vec3 Up() const;
            glm::vec3 Right() const;
            glm::mat4 LookAt() const;
            glm::mat4 Projection(float screenWidth, float screenHeight) const;
            glm::mat4 View() const;
            float Fov() const;
            void SetFov(float fov);
            float Near() const;
            void SetNear(float near);
            float Far() const;
            void SetFar(float far);

        public:
            glm::vec3 position;
            glm::vec3 target;

        private:
            float m_fov;
            float m_near;
            float m_far;
    };

}
