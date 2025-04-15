#include "scene.hpp"
#include "util/uuid.hpp"
#include <memory>
#include <optional>

namespace tiny_cherno {

    void Scene::SpawnEntity(std::shared_ptr<Entity> entity) {
        entities[entity->Uuid] = entity;
    }

    std::optional<std::shared_ptr<Entity>> Scene::GetEntityByUUID(const UUID &uuid) {
        auto it = entities.find(uuid);
        if (it != entities.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void Scene::DespawnEntity(const UUID &uuid) {
        entities.erase(uuid);
    }
    
}
