#pragma once

#include "external/glm.hpp"

namespace Ely {
template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
};
}   // namespace Ely

// Специализация для glm::vec3
namespace std {
template <>
struct hash<glm::vec3> {
    size_t operator()(const glm::vec3& v) const noexcept {
        size_t seed = 0;
        Ely::hashCombine(seed, v.x, v.y, v.z);   // Хэшируем все компоненты
        return seed;
    }
};
}   // namespace std