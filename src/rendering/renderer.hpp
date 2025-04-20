#pragma once

#include "rendering/context.hpp"
#include "rendering/shader.hpp"

namespace tiny_cherno {

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
            ShaderProgram *CurrentProgram();
        private:
            RenderingContext *m_context;
            ShaderProgram *m_program;
    };
    
}
