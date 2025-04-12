#include "uuid.hpp"
#include "rng.hpp"
#include <cstdint>
#include <ostream>

namespace tiny_cherno {

UUID::UUID() : m_value(tiny_cherno::random<uint64_t>()) {}
uint64_t UUID::value() const { return m_value; }

std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
    return os << uuid.m_value;
}

}
