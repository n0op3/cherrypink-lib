#pragma once

#include <iosfwd>
#include <sys/types.h>

namespace tiny_cherno {

class UUID {
    public:
        UUID();
        u_int64_t value() const;
        friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);

    private:
        const u_int64_t m_value;
};

}
