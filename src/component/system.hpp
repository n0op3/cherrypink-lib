#pragma once

#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "entity/entity.hpp"

namespace tiny_cherno {

template<typename T>
class ComponentSystem {
    public:
        virtual void ProcessComponent(Entity &entity, T& component) = 0;
};

class Systems {
    public:
        template<typename T>
            void RegisterSystem(std::shared_ptr<ComponentSystem<T>> system) {
                m_systems[std::type_index(typeid(T))] = system;
            }

        template<typename T>
            bool HasRegisteredSystem() {
                return HasRegisteredSystem(std::type_index(typeid(T)));
            }

        bool HasRegisteredSystem(const std::type_index index) const {
            return m_systems.find(index) != m_systems.end();
        }

        template<typename T>
            void UnregisterSystem() {
                m_systems.erase(std::type_index(typeid(T)));
            }

        template<typename T>
            bool ProcessComponent(Entity &entity, T &component) {
                auto it = m_systems.find(std::type_index(component.type()));
                if (it != m_systems.end()) {
                    std::static_pointer_cast<ComponentSystem<T>>(it->second)->ProcessComponent(entity, component);
                    return true;
                }
                return false;
            }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<ComponentSystem<std::any>>> m_systems;
};

}
