#pragma once

#include "GLFW/glfw3.h"
#include "rendering/context.hpp"
#include "rendering/shader.hpp"

namespace tiny_cherno {

    
    class OpenGLContext : public RenderingContext {
        public:
            OpenGLContext(GLFWwindow *window) : RenderingContext(window) {}
            ~OpenGLContext() override {}
        
            bool Init() override;
            void Clear() override;
            void SwapBuffers() override;
            Mesh CreateMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) override;
            void DrawMesh(const Mesh &mesh) override;
    };

    class OpenGLShader : public Shader {
        public:
            OpenGLShader(unsigned int type, const char *code);
            ~OpenGLShader() {}
    };

    class OpenGLShaderProgram : public ShaderProgram {
        public:
            OpenGLShaderProgram(OpenGLShader *vertexShader, OpenGLShader *fragmentShader);
            void Enable() override;
            void Disable() override;
    };

}
