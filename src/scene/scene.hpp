#pragma once

#include <memory>
#include <vector>
#include "entity/entity.hpp"

namespace tiny_cherno {

class Scene {
    public:
        void Update();
    public:
        std::vector<std::shared_ptr<Entity>> Entities;
};

}
