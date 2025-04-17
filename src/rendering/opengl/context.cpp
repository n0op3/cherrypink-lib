#include "glad/glad.h"

#include "rendering/context.hpp"
#include "rendering/mesh.hpp"
#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"
#include <algorithm>
#include <vector>

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

        void Clear() override {
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void SwapBuffers() override {
            glfwSwapBuffers(m_windowHandle);
        }

        Mesh CreateMesh(const std::vector<float> &&vertices, const std::vector<int> &&indices) override {
            unsigned int vao = 0, vbo = 0, indexBuffer = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            float *vertexArray = new float[vertices.size()];
            std::copy(vertices.begin(), vertices.end(), vertexArray);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

            glGenBuffers(1, &indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

            float *indexArray = new float[indices.size()];
            std::copy(indices.begin(), indices.end(), indexArray);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

            return Mesh(vao, vbo, vertices, indices);
        }

        void DrawMesh(const Mesh &mesh) override {
            glBindVertexArray(mesh.VAO());
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        }
    };

}
