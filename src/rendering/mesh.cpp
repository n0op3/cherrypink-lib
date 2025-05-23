#include "mesh.hpp"
#include "cherry_pink.hpp"
#include <glm/ext/vector_float3.hpp>
#include <vector>

namespace cherrypink {

    Mesh::Mesh(int vao, int vbo, const std::vector<float> vertices, const std::vector<unsigned int> indices) : m_indices(indices), m_vao(vao), m_vbo(vbo) {
        TC_CRITICAL_ASSERT(vertices.size() % 3 == 0, "vertex count is not a multiple of 3");

        for (size_t i = 0; i < vertices.size(); i += 3) {
            m_vertices.emplace_back(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
    }

    int Mesh::VertexCount() const { return m_vertices.size(); }

    int Mesh::Indices() const { return m_indices.size(); }

    int Mesh::VAO() const { return m_vao; }

    int Mesh::VBO() const { return m_vbo; }

}
