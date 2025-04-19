#pragma once

namespace tiny_cherno {

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
            unsigned int ID() const { return m_id; }
        protected:
            unsigned int m_id;
    };

}
