#pragma once

#include "entity/entity.hpp"
#include "util/uuid.hpp"
#include <any>
#include <functional>
#include <optional>
#include <typeindex>
#include <unordered_map>

namespace tiny_cherno {

using ComponentMap = std::unordered_map<std::type_index, std::unordered_map<UUID, std::any>>;
class ComponentRegistry {
    public:
        template<typename T>
            void AttachComponent(const Entity &entity, const T& component) {
                if (!HasComponent<T>(entity)) {
                    std::type_index index = std::type_index(typeid(T));
                    m_components[index][entity.Uuid] = std::any(component);
                }
            }

        template<typename T>
            bool HasComponent(const Entity &entity) const {
                std::type_index index = std::type_index(typeid(T));
                auto it = m_components.find(index);

                return it != m_components.end() && it->second.contains(entity.Uuid);
            }

        template<typename T>
            void RemoveComponent(const Entity &entity) {
                if (HasComponent<T>(entity)) {
                    std::type_index index = std::type_index(typeid(T));
                    m_components[index].erase(entity.Uuid);
                }
            }

        template<typename T>
            std::optional<std::reference_wrapper<T>> GetComponent(const Entity &entity) {
                if (!HasComponent<T>(entity))
                    return std::nullopt;

                std::type_index index = std::type_index(typeid(T));
                T& component = std::any_cast<T&>(m_components[index][entity.Uuid]);
                return std::ref(component);
            }

    private:
        ComponentMap m_components;
};

}
