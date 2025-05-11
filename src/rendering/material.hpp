#pragma once

#include <algorithm>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace cherrypink {

class RGBColor {
public:
    RGBColor() : RGBColor(0, 0, 0) {}
    RGBColor(float r, float g, float b)
        : m_red(std::max(0.0f, std::min(r, 1.0f))),
          m_green(std::max(0.0f, std::min(g, 1.0f))),
          m_blue(std::max(0.0f, std::min(b, 1.0f))) {}
    float Red() const { return m_red; }
    float Green() const { return m_green; }
    float Blue() const { return m_blue; }
    void SetRed(float r) { m_red = std::max(0.0f, std::min(r, 1.0f)); }
    void SetGreen(float g) { m_green = std::max(0.0f, std::min(g, 1.0f)); }
    void SetBlue(float b) { m_blue = std::max(0.0f, std::min(b, 1.0f)); }
    glm::vec3 ToVec() const { return glm::vec3(m_red, m_green, m_blue); };

private:
    float m_red;
    float m_green;
    float m_blue;
};

class RGBAColor : public RGBColor {
public:
    RGBAColor() : RGBAColor(0, 0, 0, 1) {}
    RGBAColor(float r, float g, float b) : RGBColor(r, g, b), m_alpha(1) {}
    RGBAColor(float r, float g, float b, float a)
        : RGBColor(r, g, b), m_alpha(std::max(0.0f, std::min(a, 1.0f))) {}
    float Alpha() const;
    void SetAlpha(float a) { m_alpha = std::max(0.0f, std::min(a, 1.0f)); }
    glm::vec4 ToVec() const {
        return glm::vec4(Red(), Green(), Blue(), m_alpha);
    };

private:
    float m_alpha;
};

struct ShaderMaterial {
    RGBAColor color;
};

}  // namespace cherrypink
