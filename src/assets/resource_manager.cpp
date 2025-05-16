#include "resource_manager.hpp"
#include "assimp/vector3.h"
#include "cherry_pink.hpp"
#include "rendering/mesh.hpp"
#include "spdlog/spdlog.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <utility>

namespace cherrypink {

    std::optional<Mesh *>
        ResourceManager::LoadMesh(const std::filesystem::path &path) {
            if (m_meshCache.contains(path))
                return &m_meshCache.at(path);

            static Assimp::Importer importer;
            const aiScene *assimpScene = importer.ReadFile(
                    m_resourceDirectory / path,
                    aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

            spdlog::debug("Loaded a mesh with {} vertices",
                    assimpScene->mMeshes[0]->mNumVertices);

            std::vector<float> vertices;
            for (unsigned int vertexIndex = 0;
                    vertexIndex < assimpScene->mMeshes[0]->mNumVertices; vertexIndex++) {
                aiVector3f vertex = assimpScene->mMeshes[0]->mVertices[vertexIndex];
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
            }

            std::vector<unsigned int> indices;
            for (unsigned int indexIndex = 0;
                    indexIndex < assimpScene->mMeshes[0]->mNumFaces; indexIndex++) {
                aiFace *face = &assimpScene->mMeshes[0]->mFaces[indexIndex];
                for (unsigned int i = 0; i < face->mNumIndices; i++) {
                    indices.push_back(face->mIndices[i]);
                }
            }

            Mesh mesh =
                cherrypink::GetRenderer().Context()->CreateMesh(vertices, indices);

            m_meshCache.insert(std::make_pair(std::filesystem::path(path), mesh));

            return &m_meshCache.at(path);
        }

    std::optional<Texture *>
        ResourceManager::LoadTexture(const std::filesystem::path &path) {
            if (m_textureIndices.contains(path))
                return &m_textures.at(m_textureIndices.at(path));

            int width = 0;
            int height = 0;
            int channels = 3;

            unsigned char *image = stbi_load(path.c_str(), &width, &height, &channels, 0);

            if (image == nullptr)
                return std::nullopt;

            Texture texture = cherrypink::GetRenderer().Context()->CreateTexture(
                    {(uint32_t)width, (uint32_t)height, (uint8_t)channels}, image);

            m_textureIndices.insert_or_assign(path, texture.Id());
            m_textures.insert_or_assign(texture.Id(), texture);

            return &texture;
        }

    void ResourceManager::Shutdown() { m_meshCache.clear(); }

} // namespace cherrypink
