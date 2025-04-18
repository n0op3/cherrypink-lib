#pragma once

#include "rendering/context.hpp"
#include "rendering/shader.hpp"

namespace tiny_cherno {

    class Renderer {
        public:
            Renderer(RenderingContext *context, ShaderProgram *program);
            void Prepare();
            void Finish();
            void Shutdown();
        private:
            RenderingContext *m_context;
            ShaderProgram *m_program;
    };
    
}
