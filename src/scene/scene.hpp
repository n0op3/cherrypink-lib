#pragma once

#include <memory>
#include <vector>
#include "component/component.hpp"
#include "entity/entity.hpp"

namespace tiny_cherno {

class Scene {
    public:
        std::vector<std::shared_ptr<Entity>> entities;
        ComponentRegistry componentRegistry;
};

}
