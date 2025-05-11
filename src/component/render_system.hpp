#pragma once

#include "cherry_pink.hpp"
#include "component/system.hpp"
#include "rendering/mesh.hpp"

namespace cherrypink {

class RenderingSystem : public RenderSystem<Mesh> {
    void ProcessComponent(const UUID &entityUuid, Mesh &mesh,
                          double partialTicks) override {
        if (cherrypink::CurrentScene()
                .componentRegistry.GetComponent<Transform>(entityUuid) !=
            std::nullopt) {
            const Transform *transform =
                *CurrentScene().componentRegistry.GetComponent<Transform>(
                    entityUuid);

            if (CurrentScene().componentRegistry.GetComponent<ShaderMaterial>(
                    entityUuid) != std::nullopt) {
                const ShaderMaterial &material =
                    *CurrentScene()
                         .componentRegistry
                         .GetComponent<ShaderMaterial>(entityUuid)
                         .value();

                cherrypink::GetRenderer().CurrentProgram()->SetUniformMaterial(
                    "material", material);
            }

            cherrypink::GetRenderer().DrawMesh(*transform, mesh, partialTicks);
        }
    }
};

}  // namespace cherrypink
