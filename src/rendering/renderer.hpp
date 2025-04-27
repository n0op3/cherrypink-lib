#pragma once

#include "component/transform_component.hpp"
#include "rendering/camera.hpp"
#include "rendering/context.hpp"
#include "rendering/mesh.hpp"
#include "rendering/shader.hpp"

namespace cherrypink {

    class Renderer {
        public:
            Renderer(RenderingContext *context, ShaderProgram *program);
            Renderer(RenderingContext *context);
            void Prepare();
            void Finish();
            void Shutdown();
            void SetContext(RenderingContext *context);
            RenderingContext *Context();
            void SetProgram(ShaderProgram *context);
            void UseCamera(Camera *camera);
            void DrawMesh(const TransformComponent &transform, const Mesh &mesh, float partialTicks);
            ShaderProgram *CurrentProgram();
        private:
            RenderingContext *m_context;
            ShaderProgram *m_program;
            Camera *m_camera;
    };
    
}
