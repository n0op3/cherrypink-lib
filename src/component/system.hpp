#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "util/uuid.hpp"

namespace cherrypink {

template <typename T>
class UpdateSystem {
public:
    virtual void ProcessComponent(const UUID& entityUuid, T& object,
                                  double deltaTime) = 0;
};

template <typename T>
class RenderSystem {
public:
    virtual void ProcessComponent(const UUID& entityUuid, T& object,
                                  double partialTicks) = 0;
};

class SystemRegistry {
public:
    template <typename T>
    void RegisterUpdateSystem(std::shared_ptr<UpdateSystem<T>> system) {
        m_updateSystems[std::type_index(typeid(T))].push_back(system);
    }

    template <typename T>
    void RegisterRenderSystem(std::shared_ptr<RenderSystem<T>> system) {
        m_renderSystems[std::type_index(typeid(T))].push_back(system);
    }

    template <typename T>
    void ProcessUpdateComponent(const UUID& entityUuid, T& component,
                                double deltaTime) {
        auto it = m_updateSystems.find(std::type_index(typeid(T)));
        if (it != m_updateSystems.end()) {
            for (auto system : it->second) {
                auto processor =
                    std::static_pointer_cast<UpdateSystem<T>>(system);
                processor->ProcessComponent(entityUuid, component, deltaTime);
            }
        }
    }

    template <typename T>
    void ProcessRenderComponent(const UUID& entityUuid, T& component,
                                double partialTicks) {
        auto it = m_renderSystems.find(std::type_index(typeid(T)));
        if (it != m_renderSystems.end()) {
            for (auto system : it->second) {
                auto processor =
                    std::static_pointer_cast<UpdateSystem<T>>(system);
                processor->ProcessComponent(entityUuid, component,
                                            partialTicks);
            }
        }
    }

    void Shutdown() {
        m_updateSystems.clear();
        m_renderSystems.clear();
    }

private:
    using SystemsMap =
        std::unordered_map<std::type_index, std::vector<std::shared_ptr<void>>>;

    SystemsMap m_updateSystems;
    SystemsMap m_renderSystems;
};

}  // namespace cherrypink
