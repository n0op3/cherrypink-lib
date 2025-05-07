#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include <algorithm>
namespace cherrypink {

    class RGBColor {
        public:
            RGBColor() : RGBColor(0, 0, 0) {}
            RGBColor(float r, float g, float b) : m_red(std::max(0.0f, std::min(r, 1.0f))), m_green(std::max(0.0f, std::min(r, 1.0f))), m_blue(std::max(0.0f, std::min(r, 1.0f))) {}
            float Red()   const { return m_red; }
            float Green() const { return m_green; }
            float Blue()  const { return m_blue; }
            glm::vec3 ToVec() const { return glm::vec3(m_red, m_green, m_blue); };
        private:
            const float m_red;
            const float m_green;
            const float m_blue;
    };

    class RGBAColor : public RGBColor {
        public:
            RGBAColor() : RGBAColor(0, 0, 0, 1) {}
            RGBAColor(float r, float g, float b, float a) : RGBColor(r, g, b), m_alpha(std::max(0.0f, std::min(a, 1.0f))) {}
            float Alpha() const;
            glm::vec4 ToVec() const { return glm::vec4(Red(), Green(), Blue(), m_alpha); };
        private:
            const float m_alpha;
    };

    struct ShaderMaterial {
        RGBAColor color;
    };

}
