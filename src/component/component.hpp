// HACK: This whole file is one big brainf**k. Proceed at your own risk.

#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "component/system.hpp"
#include "util/uuid.hpp"

namespace cherrypink {

struct ComponentWrapper {
    virtual void ProcessUpdate(const UUID& entityUuid,
                               SystemRegistry& registry) = 0;
    virtual void ProcessRender(const UUID& entityUuid,
                               SystemRegistry& registry) = 0;
};

template <typename T>
struct TypedObjectWrapper : ComponentWrapper {
    T component;

    TypedObjectWrapper(T object) : component(std::move(object)) {}

    void ProcessUpdate(const UUID& entityUuid,
                       SystemRegistry& registry) override {
        registry.ProcessUpdateComponent(entityUuid, component);
    }

    void ProcessRender(const UUID& entityUuid,
                       SystemRegistry& registry) override {
        registry.ProcessRenderComponent(entityUuid, component);
    }
};

class ComponentRegistry {
public:
    template <typename T>
    void AttachComponent(const UUID& uuid, T comp) {
        m_components[std::type_index(typeid(T))][uuid] =
            std::make_shared<TypedObjectWrapper<T>>(std::move(comp));
    }

    template <typename T>
    T* AttachComponent(const UUID& uuid) {
        m_components[std::type_index(typeid(T))][uuid] =
            std::make_shared<TypedObjectWrapper<T>>(std::move(T()));

        return *GetComponent<T>(uuid);
    }

    template <typename T>
    std::optional<T*> GetComponent(const UUID& uuid) {
        auto& component_map = m_components[std::type_index(typeid(T))];
        if (auto it = component_map.find(uuid); it != component_map.end()) {
            TypedObjectWrapper<T>* typed_wrapper =
                static_cast<TypedObjectWrapper<T>*>(it->second.get());

            if (typed_wrapper) {
                return &typed_wrapper->component;
            }
        }
        return std::nullopt;
    }

    template <typename T>
    std::vector<std::pair<UUID, T*>> GetComponents() {
        std::vector<std::pair<UUID, T*>> components;
        for (auto map : m_components[std::type_index(typeid(T))]) {
            TypedObjectWrapper<T>* typed_wrapper =
                static_cast<TypedObjectWrapper<T>*>(map.second.get());

            if (typed_wrapper) {
                components.push_back(
                    std::pair(map.first, &typed_wrapper->component));
            }
        }

        return components;
    }

    void updateComponents(SystemRegistry& systems) const {
        for (auto& [type, entity_map] : m_components) {
            for (auto& [entity, wrapper] : entity_map) {
                wrapper->ProcessUpdate(entity, systems);
            }
        }
    }

    void updateRenderComponents(SystemRegistry& systems) const {
        for (auto& [type, entity_map] : m_components) {
            for (auto& [entity, wrapper] : entity_map) {
                wrapper->ProcessRender(entity, systems);
            }
        }
    }

private:
    std::unordered_map<
        std::type_index,
        std::unordered_map<UUID, std::shared_ptr<ComponentWrapper>>>
        m_components;
};

}  // namespace cherrypink
