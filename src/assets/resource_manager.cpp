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
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
            for (unsigned int i = 0; i < assimpScene->mMeshes[0]->mNumVertices; i++) {
                aiVector3f vertex = assimpScene->mMeshes[0]->mVertices[i];
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
            }

            std::vector<unsigned int> indices;
            for (unsigned int i = 0; i < assimpScene->mMeshes[0]->mNumFaces; i++) {
                aiFace *face = &assimpScene->mMeshes[0]->mFaces[i];

                for (unsigned int j = 0; j < face->mNumIndices; j++) {
                    indices.push_back(face->mIndices[j]);
                }
            }

            std::vector<float> textureCoords(vertices.size() / 3 * 2);
            if (assimpScene->mMeshes[0]->HasTextureCoords(0)) {
                textureCoords.reserve(assimpScene->mMeshes[0]->mNumVertices * 2);
                for (unsigned int i = 0; i < assimpScene->mMeshes[0]->mNumVertices; i++) {
                    textureCoords.push_back(assimpScene->mMeshes[0]->mTextureCoords[0][i].x);
                    textureCoords.push_back(assimpScene->mMeshes[0]->mTextureCoords[0][i].y);
                }
            }

            m_meshCache.emplace(std::filesystem::path(path),
                    cherrypink::GetRenderer().Context()->CreateMesh(
                        vertices, indices, textureCoords));

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
