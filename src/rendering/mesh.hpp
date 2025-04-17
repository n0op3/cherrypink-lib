#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace tiny_cherno {

    class Mesh {
        public:
            Mesh(int vao, int vbo, const std::vector<float> vertices, const std::vector<int> indices);
            int VertexCount() const;
            int VAO() const;
            int VBO() const;
        private:
            std::vector<glm::vec3> m_vertices;
            std::vector<int> m_indices;
            int m_vao;
            int m_vbo;
    };

}
