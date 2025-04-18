#include "glad/glad.h"

#include "rendering/opengl/opengl.hpp"
#include "spdlog/spdlog.h"
#include <cstddef>

namespace tiny_cherno {

    OpenGLShader::OpenGLShader(unsigned int type, const char *code) {
        m_id = glCreateShader(type);
        const GLchar *source = (const GLchar *)code;
        glShaderSource(m_id, 1, &source, NULL);
        glCompileShader(m_id);

        GLint success;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(m_id, 512, NULL, infoLog);
            spdlog::error("OpenGL shader compilation failed: {}", infoLog);
        }
    }

    OpenGLShaderProgram::OpenGLShaderProgram(OpenGLShader *vertexShader, OpenGLShader *fragmentShader) {
        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShader->ID());
        glAttachShader(m_id, fragmentShader->ID());
        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            spdlog::error("OpenGL program linking failed: {}", infoLog);
        }
    }

    void OpenGLShaderProgram::Enable() {
        glUseProgram(m_id);
    }

    void OpenGLShaderProgram::Disable() {
        glUseProgram(0);
    }

}
