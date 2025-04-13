#pragma once

#include "entity/entity.hpp"
#include "util/uuid.hpp"
#include <memory>
#include <spdlog/spdlog.h>
#include <typeindex>
#include <unordered_map>

namespace tiny_cherno {

template<typename T>
class System {
public:
    virtual void ProcessComponent(const UUID& entityUuid, T& object) = 0;
    virtual ~System() = default;
};

class SystemRegistry {
public:
    template<typename T>
    void RegisterSystem(std::shared_ptr<System<T>> system) {
        m_systems[std::type_index(typeid(T))] = system;
    }

    template<typename T>
    void process(const UUID& entityUuid, T& component) {
        auto it = m_systems.find(std::type_index(typeid(T)));
        if (it != m_systems.end()) {
            auto processor = std::static_pointer_cast<System<T>>(it->second);
            processor->ProcessComponent(entityUuid, component);
        }
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> m_systems;
};

struct ObjectWrapper {
    virtual ~ObjectWrapper() = default;
    virtual void Process(const UUID& entityUuid, SystemRegistry& registry) = 0;
};

template<typename T>
struct TypedObjectWrapper : ObjectWrapper {
    T component;
    TypedObjectWrapper(T object) : component(std::move(object)) {}
    void Process(const UUID& entityUuid, SystemRegistry& registry) override {
        registry.process(entityUuid, component);
    }
};

class ComponentRegistry {
private:
    std::unordered_map<std::type_index, std::unordered_map<UUID, std::unique_ptr<ObjectWrapper>>> m_components;
    SystemRegistry m_systems;

public:
    template<typename T>
    void AttachComponent(Entity entity, T comp) {
        m_components[std::type_index(typeid(T))][entity.Uuid] = std::make_unique<TypedObjectWrapper<T>>(std::move(comp));
    }

    template<typename T>
    std::optional<T*> GetComponent(Entity entity) {
        auto& component_map = m_components[std::type_index(typeid(T))];
        if (auto it = component_map.find(entity.Uuid); it != component_map.end()) {
            TypedObjectWrapper<T>* typed_wrapper = static_cast<TypedObjectWrapper<T>*>(it->second.get());
            if (typed_wrapper) {
                return &typed_wrapper->component;
            }
        }
        return std::nullopt;
    }

    void updateComponents() {
        for (auto& [type, entity_map] : m_components) {
            for (auto& [entity, wrapper] : entity_map) {
                wrapper->Process(entity, m_systems);
            }
        }
    }

    SystemRegistry& getRegistry() { return m_systems; }
};


} // namespace tiny_cherno
