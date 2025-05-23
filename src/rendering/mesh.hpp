#pragma once

#include "glm/ext/vector_float2.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace cherrypink {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 textureCoord;
    };

    class Mesh {
        public:
            Mesh(int vao, int vbo, const std::vector<float> vertices, const std::vector<unsigned int> indices);
            int VertexCount() const;
            int Indices() const;
            int VAO() const;
            int VBO() const;
        private:
            std::vector<glm::vec3> m_vertices;
            std::vector<unsigned int> m_indices;
            int m_vao;
            int m_vbo;
    };

}
