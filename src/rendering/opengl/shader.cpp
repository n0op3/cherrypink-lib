#include "glad/glad.h"

#include "rendering/opengl/opengl.hpp"
#include "spdlog/spdlog.h"
#include <cstddef>
#include <glm/gtc/type_ptr.hpp>

namespace cherrypink {

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

    void OpenGLShaderProgram::SetUniform1f(const char *uniformName, float value) {
        glUniform1f(glGetUniformLocation(m_id, uniformName), value);
    }

    void OpenGLShaderProgram::SetUniform2f(const char *uniformName, glm::vec2 &&value) {
        glUniform2f(glGetUniformLocation(m_id, uniformName), value.x, value.y);
    }

    void OpenGLShaderProgram::SetUniform3f(const char *uniformName, glm::vec3 &&value) {
        glUniform3f(glGetUniformLocation(m_id, uniformName), value.x, value.y, value.z);
    }

    void OpenGLShaderProgram::SetUniform4f(const char *uniformName, glm::vec4 &&value) {
        glUniform4f(glGetUniformLocation(m_id, uniformName), value.x, value.y, value.z, value.w);
    }

    void OpenGLShaderProgram::SetUniform2x2f(const char *uniformName, glm::mat2x2 &&value) {
        glUniformMatrix2fv(glGetUniformLocation(m_id, uniformName), 1, false, glm::value_ptr(value));
    }

    void OpenGLShaderProgram::SetUniform3x3f(const char *uniformName, glm::mat3x3 &&value) {
        glUniformMatrix3fv(glGetUniformLocation(m_id, uniformName), 1, false, glm::value_ptr(value));
    }

    void OpenGLShaderProgram::SetUniform4x4f(const char *uniformName, glm::mat4x4 &&value) {
        glUniformMatrix4fv(glGetUniformLocation(m_id, uniformName), 1, false, glm::value_ptr(value));
    }

}
