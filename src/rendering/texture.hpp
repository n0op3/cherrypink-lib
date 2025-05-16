#pragma once

#include <cstdint>

namespace cherrypink {

    struct TextureParams {
        uint32_t width;
        uint32_t height;
        uint8_t channels;
    };

    class Texture {
        public:
            Texture(const TextureParams params, uint32_t id)
                : m_id(id), m_width(params.width),
                m_height(params.height), m_channels(params.channels) {
                    m_data = new unsigned char[m_width * m_height * m_channels];
                    custom = true;
                }

            Texture(const TextureParams params, unsigned char *data, uint32_t id) : cherrypink::Texture(params, id) { m_data = data; }
            ~Texture() { if (custom) delete m_data; }

            uint32_t Id() const { return m_id; }
            uint32_t Width() const { return m_width; }
            uint32_t Height() const { return m_height; }
            uint8_t Channels() const { return m_channels; }
            unsigned char* Data() const { return m_data; }
            bool IsCustom() { return custom; }

        private:
            uint32_t m_id;
            uint32_t m_width = 0;
            uint32_t m_height = 0;
            uint8_t m_channels = 0;
            unsigned char *m_data = nullptr;
            bool custom = false;
    };

} // namespace cherrypink
