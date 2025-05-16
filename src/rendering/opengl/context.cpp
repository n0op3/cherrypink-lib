#include "glad/glad.h"

#include "rendering/mesh.hpp"
#include "rendering/opengl/opengl.hpp"
#include "rendering/texture.hpp"
#include "spdlog/spdlog.h"

#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <algorithm>
#include <vector>

namespace cherrypink {

    bool OpenGLContext::Init() {
        glfwMakeContextCurrent(m_window->Handle());
        if (!gladLoadGL()) {
            spdlog::critical("Failed to initialize OpenGL");
            return false;
        }
        return true;
    }

    void OpenGLContext::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_window->Handle()); }

    Mesh OpenGLContext::CreateMesh(const std::vector<float> &vertices,
            const std::vector<unsigned int> &indices) {
        unsigned int vao = 0, vbo = 0, ebo = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        float *vertexArray = new float[vertices.size()];
        std::copy(vertices.begin(), vertices.end(), vertexArray);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertexArray,
                GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        unsigned int *indexArray = new unsigned int[indices.size()];
        std::copy(indices.begin(), indices.end(), indexArray);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                indexArray, GL_STATIC_DRAW);

        return Mesh(vao, vbo, vertices, indices);
    }

    Texture OpenGLContext::CreateTexture(TextureParams params,
            unsigned char *data) {
        GLuint textureId = 0;

        glGenTextures(1, &textureId);

        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLuint channels = GL_RGB;
        switch (params.channels) {
            case 1: channels = GL_R; break;
            case 2: channels = GL_RG; break;
            case 3: channels = GL_RGB; break;
            case 4: channels = GL_RGBA; break;
            default:
                spdlog::warn("Unsupported number of channels: {}, falling back to RGB",
                        params.channels);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, channels, params.width, params.height, 0, channels, GL_UNSIGNED_BYTE, data);

        return Texture(params, data, textureId);
    }

    void OpenGLContext::DrawMesh(const Mesh &mesh) {
        glDrawElements(GL_TRIANGLES, mesh.Indices(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLContext::SetViewport(unsigned int x, unsigned int y,
            unsigned int width, unsigned int height) {
        glViewport(x, y, width, height);
    }

} // namespace cherrypink
