#include "renderer.hpp"
#include "rendering/context.hpp"
#include "rendering/shader.hpp"

namespace tiny_cherno {

    Renderer::Renderer(RenderingContext *context, ShaderProgram *program) : m_context(context), m_program(program) {}

    Renderer::Renderer(RenderingContext *context) : m_context(context) {}

    void Renderer::Prepare() {
        m_context->Clear();
        m_program->Enable();
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

    void Renderer::SetContext(RenderingContext *context) { m_context = context; }

    RenderingContext *Renderer::Context() { return m_context; }

    void Renderer::SetProgram(ShaderProgram *shaderProgram) { m_program = shaderProgram; }

    ShaderProgram *Renderer::CurrentProgram() { return m_program; }

}

