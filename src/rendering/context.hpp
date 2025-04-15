#pragma once

struct GLFWwindow;

namespace tiny_cherno {

    class RenderingContext {
        public:
            RenderingContext(GLFWwindow *window) : m_windowHandle(window) {}
            virtual bool Init() = 0;
            virtual void SwapBuffers() = 0;
        protected:
            GLFWwindow *m_windowHandle;
    };

}
