#include "renderer.hpp"

namespace tiny_cherno {

    Renderer::Renderer(RenderingContext *context, ShaderProgram *program) : m_context(context), m_program(program) {}

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

}
