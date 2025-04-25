#include "camera.hpp"
#include <glm/detail/qualifier.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

namespace tiny_cherno {

    static glm::vec3 s_cameraUp = glm::vec3(0.0, 1.0, 0.0);

    glm::vec3 Camera::Direction() const {
        return glm::normalize(position - target);
    }

    glm::vec3 Camera::Up() const {
        return glm::cross(Direction(), Right());
    }

    glm::vec3 Camera::Right() const {
        return glm::normalize(glm::cross(s_cameraUp, Direction()));
    }

    glm::mat4 Camera::LookAt() const {
        return glm::lookAt(position, target, s_cameraUp);
    }

    glm::mat4 Camera::Projection(float screenWidth, float screenHeight) const {
        return glm::perspective(glm::radians(m_fov), screenHeight / screenHeight, m_near, m_far);
    }

    glm::mat4 Camera::View() const {
        return LookAt(); // TODO: impl euler angles
    }

    float Camera::Fov() const {
        return m_fov;
    }

    void Camera::SetFov(float fov) {
        if (fov > 0 && fov <= 360)
            m_fov = fov;
    }

    float Camera::Near() const {
        return m_near;
    }

    void Camera::SetNear(float near) {
        if (near > 0 && near < m_far)
            m_near = near;
    }

    float Camera::Far() const {
        return m_far;
    }

    void Camera::SetFar(float far) {
        if (far > 0 && m_far < m_far)
            m_far = far;
    }

}
