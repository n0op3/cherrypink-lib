#pragma once

#include <cstdint>
#include <iosfwd>

namespace tiny_cherno {

class UUID {
    public:
        UUID();
        uint64_t Value() const;
        friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);

    private:
        const uint64_t m_value;
};

}
