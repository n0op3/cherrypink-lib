#pragma once

#include <memory>
#include <optional>
#include "component/component.hpp"
#include "entity/entity.hpp"
#include "util/uuid.hpp"

namespace tiny_cherno {

class Scene {
    public:
        void SpawnEntity(std::shared_ptr<Entity> entity);
        std::optional<std::shared_ptr<Entity>> GetEntityByUUID(const UUID &uuid);
    public:
        std::unordered_map<UUID, std::shared_ptr<Entity>> entities;
        ComponentRegistry componentRegistry;
};

}
