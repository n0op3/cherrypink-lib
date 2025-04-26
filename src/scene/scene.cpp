#include "scene.hpp"
#include "entity/entity.hpp"
#include "util/uuid.hpp"
#include <memory>
#include <optional>

namespace cherrypink {

    Entity &Scene::SpawnEntity() {
        auto ent = std::make_shared<Entity>();
        entities[ent->Uuid] = ent;
        return *entities[ent->Uuid];
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
