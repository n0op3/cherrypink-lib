#include "glad/glad.h"

#include "rendering/context.hpp"
#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"

namespace tiny_cherno {

    class OpenGLContext : public RenderingContext {
        public:
            OpenGLContext(GLFWwindow *window) : RenderingContext(window) {}

        bool Init() override {
            glfwMakeContextCurrent(m_windowHandle);
            if (!gladLoadGL()) {
                spdlog::critical("Failed to initialize OpenGL");
                return false;
            }
            return true;
        }

        void SwapBuffers() override {
            glfwSwapBuffers(m_windowHandle);
        }
    };

}
