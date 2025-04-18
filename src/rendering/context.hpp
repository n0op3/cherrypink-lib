#pragma once

#include "mesh.hpp"
#include <vector>

struct GLFWwindow;

namespace tiny_cherno {

    class RenderingContext {
        public:
            RenderingContext(GLFWwindow *window) : m_windowHandle(window) {}
            virtual bool Init() = 0;
            virtual void Clear() = 0;
            virtual void SwapBuffers() = 0;
            virtual Mesh CreateMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) = 0;
            virtual void DrawMesh(const Mesh &mesh) = 0;
        protected:
            GLFWwindow *m_windowHandle;
    };

}
