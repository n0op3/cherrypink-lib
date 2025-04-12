#include "scene.hpp"
#include "spdlog/spdlog.h"
#include <memory>

namespace tiny_cherno {

    void Scene::Update() {
        for (const std::shared_ptr<Entity> &entity : Entities) {
            spdlog::info("Updating entity with UUID {}", entity->Uuid.value());
        }
    }

}
