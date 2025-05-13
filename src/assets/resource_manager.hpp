#pragma once

#include <filesystem>
#include <optional>
#include <unordered_map>

#include "rendering/mesh.hpp"

namespace cherrypink {

    class ResourceManager {
        public:
            ResourceManager(
                    std::filesystem::path resourceDirectory) // We're copying the path to
                                                             // avoid changing the resource
                                                             // directory during runtime
                : m_resourceDirectory(resourceDirectory) {}

            std::optional<Mesh *> LoadFromFile(const std::filesystem::path &path);
            void Shutdown();

        private:
            const std::filesystem::path m_resourceDirectory;
            std::unordered_map<std::filesystem::path, Mesh> m_meshCache;
    };

} // namespace cherrypink
