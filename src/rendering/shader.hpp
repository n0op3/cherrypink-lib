#pragma once

#include "rendering/material.hpp"
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace cherrypink {

    class Shader {
        public:
            unsigned int ID() const { return m_id; }
        protected:
            unsigned int m_id;
    };

    class ShaderProgram {
        public:
            virtual ~ShaderProgram() {}
            virtual void Enable() = 0;
            virtual void Disable() = 0;
            virtual void SetUniform1f(const char *uniformName, float value) = 0;
            virtual void SetUniform2f(const char *uniformName, glm::vec2 &&value) = 0;
            virtual void SetUniform3f(const char *uniformName, glm::vec3 &&value) = 0;
            virtual void SetUniform4f(const char *uniformName, glm::vec4 &&value) = 0;
            virtual void SetUniform2x2f(const char *uniformName, glm::mat2x2 &&value) = 0;
            virtual void SetUniform3x3f(const char *uniformName, glm::mat3x3 &&value) = 0;
            virtual void SetUniform4x4f(const char *uniformName, glm::mat4x4 &&value) = 0;
            virtual void SetUniformMaterial(const char *uniformName, const ShaderMaterial &material) = 0;
            unsigned int ID() const { return m_id; }
        protected:
            unsigned int m_id;
    };

}
