#pragma once

#include <cstdint>
#include <functional>

namespace tiny_cherno {

class UUID {
    public:
        UUID();
        uint64_t Value() const;
        friend bool operator==(const UUID& uuid, const UUID& other);

    private:
        const uint64_t m_value;
};

}

namespace std {
    template<>
    struct hash<tiny_cherno::UUID> {
        size_t operator()(const tiny_cherno::UUID& uuid) const noexcept {
            return std::hash<uint64_t>{}(uuid.Value());
        }
    };
}
