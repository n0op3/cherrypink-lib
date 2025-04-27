#include "renderer.hpp"
#include "cherry_pink.hpp"
#include "rendering/context.hpp"
#include "rendering/shader.hpp"
#include <glm/ext/matrix_float4x4.hpp>

namespace cherrypink {

    Renderer::Renderer(RenderingContext *context, ShaderProgram *program) : m_context(context), m_program(program) {}

    Renderer::Renderer(RenderingContext *context) : m_context(context) {}

    void Renderer::Prepare() {
        m_context->Clear();
        m_program->Enable();
        m_context->SetViewport(0, 0, GetWindow().Width(), GetWindow().Height());
        if (m_camera) {
            m_program->SetUniform4x4f("cameraView", m_camera->View());
            m_program->SetUniform4x4f("cameraProjection", m_camera->Projection(m_context->GetWindow()->Width(), m_context->GetWindow()->Height()));
        }
    }

    void Renderer::Finish() {
        m_context->SwapBuffers();
        m_program->Disable();
    }

    void Renderer::Shutdown() {
        delete m_context;
        delete m_program;
        m_context = nullptr;
        m_program = nullptr;
    }

    void Renderer::DrawMesh(const TransformComponent &transform, const Mesh &mesh, float partialTicks) {
        m_program->SetUniform4x4f("transform", transform.ModelMatrix(partialTicks));
        m_context->DrawMesh(mesh);
    }

    void Renderer::SetContext(RenderingContext *context) { m_context = context; }

    RenderingContext *Renderer::Context() { return m_context; }

    void Renderer::SetProgram(ShaderProgram *shaderProgram) { m_program = shaderProgram; }

    void Renderer::UseCamera(Camera *camera) { m_camera = camera; }

    ShaderProgram *Renderer::CurrentProgram() { return m_program; }

}
