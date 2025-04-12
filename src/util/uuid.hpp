#pragma once

#include <iosfwd>
#include <sys/types.h>

namespace tiny_cherno {

class UUID {
    public:
        UUID();
        const u_int64_t value();
        friend std::ostream& operator<<(std::ostream& os, const UUID& uuid);

    private:
        const u_int64_t m_value;
};

}
