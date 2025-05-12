#include "resource_manager.hpp"
#include "spdlog/spdlog.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <optional>

namespace cherrypink {

    std::optional<Mesh>
        ResourceManager::LoadFromFile(const std::filesystem::path &path) {
            static Assimp::Importer importer;
            const aiScene *assimpScene = importer.ReadFile(
                    m_resourceDirectory / path,
                    aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

            spdlog::debug("Loaded a mesh with {} vertices",
                    assimpScene->mMeshes[0]->mNumVertices);

            return std::nullopt;
        }

    void ResourceManager::Shutdown() { m_meshCache.clear(); }

} // namespace cherrypink
