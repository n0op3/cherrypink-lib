#include <glad/glad.h>

#include <algorithm>
#include <cstddef>
#include <format>
#include <vector>

#include "GLFW/glfw3.h"
#include "cherry_pink.hpp"
#include "rendering/mesh.hpp"
#include "rendering/opengl/opengl.hpp"
#include "rendering/texture.hpp"
#include "spdlog/spdlog.h"

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
        glClearColor(0.1, 0.1, 0.18, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_window->Handle()); }

    Mesh OpenGLContext::CreateMesh(const std::vector<float> &vertices,
                                   const std::vector<unsigned int> &indices,
                                   const std::vector<float> &textureCoords) {
        TC_CRITICAL_ASSERT(vertices.size() % 3 == 0, "vertex count is not a multiple of 3");
        TC_CRITICAL_ASSERT(
            textureCoords.size() / 2 == vertices.size() / 3,
            std::format("texture coord count must be the same as the vertex count. V: {}, TC: {}",
                        vertices.size() / 3, textureCoords.size() / 2));

        unsigned int vao = 0, vbo = 0, ebo = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        size_t vertexCount = vertices.size() / 3;

        float *vertexData = new float[vertexCount * 5];
        for (size_t i = 0; i < vertexCount; i++) {
            vertexData[i * 5] = vertices.at(i * 3);
            vertexData[i * 5 + 1] = vertices.at(i * 3 + 1);
            vertexData[i * 5 + 2] = vertices.at(i * 3 + 2);
            vertexData[i * 5 + 3] = textureCoords.at(i * 2);
            vertexData[i * 5 + 4] = textureCoords.at(i * 2 + 1);
        }

        glBufferData(GL_ARRAY_BUFFER, (vertexCount * 5) * sizeof(float), vertexData,
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        unsigned int *indexArray = new unsigned int[indices.size()];
        std::copy(indices.begin(), indices.end(), indexArray);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indexArray,
                     GL_STATIC_DRAW);

        return Mesh(vao, vbo, vertices, indices);
    }

    Texture OpenGLContext::CreateTexture(TextureParams params, unsigned char *data) {
        GLuint textureId = 0;

        glGenTextures(1, &textureId);

        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLuint channels = GL_RGB;
        switch (params.channels) {
            case 1:
                channels = GL_R;
                break;
            case 2:
                channels = GL_RG;
                break;
            case 3:
                channels = GL_RGB;
                break;
            case 4:
                channels = GL_RGBA;
                break;
            default:
                spdlog::warn("Unsupported number of channels: {}, falling back to RGB",
                             params.channels);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, channels, params.width, params.height, 0, channels,
                     GL_UNSIGNED_BYTE, data);

        return Texture(params, data, textureId);
    }

    void OpenGLContext::DrawMesh(const Mesh &mesh) {
        glDrawElements(GL_TRIANGLES, mesh.Indices(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLContext::SetViewport(unsigned int x, unsigned int y, unsigned int width,
                                    unsigned int height) {
        glViewport(x, y, width, height);
    }

}  // namespace cherrypink
