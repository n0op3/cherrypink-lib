#include "uuid.hpp"
#include "rng.hpp"
#include <cstdint>
#include <ostream>

namespace cherrypink {

UUID::UUID() : m_value(cherrypink::random<uint64_t>()) {}
uint64_t UUID::Value() const { return m_value; }

std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
    return os << uuid.Value();
}

bool operator==(const UUID& uuid, const UUID& other) {
    return uuid.Value() == other.Value();
}

}
