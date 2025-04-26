#pragma once

#include <memory>
#include <optional>
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "rendering/camera.hpp"
#include "util/uuid.hpp"

namespace cherrypink {

class Scene {
    public:
        Scene() = default;
        Scene(const Scene &&other) = delete;
        void SpawnEntity(std::shared_ptr<Entity> entity);
        std::optional<std::shared_ptr<Entity>> GetEntityByUUID(const UUID &uuid);
        void DespawnEntity(const UUID &uuid);
    public:
        std::unordered_map<UUID, std::shared_ptr<Entity>> entities;
        ComponentRegistry componentRegistry;
        Camera camera;
};

}
