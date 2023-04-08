#include "pch.hpp"
#include "Math.hpp"


float fastInverseSqrt(float d) {
    constexpr std::uint32_t magic = 0x5f3759df;
    // reinterpret_cast doesn't appear to have too much effect compared to *(long*)&d
    const std::uint32_t i = magic - (*reinterpret_cast<std::uint32_t *>(const_cast<float *>(&d)) >> 1);
    const float y = *reinterpret_cast<float *>(const_cast<std::uint32_t *>(&i));
    return y * (1.5f - (d * 0.5f * y * y));
}

Vector magnitude(const Vector vec, const float mag) {
    return vec * fastInverseSqrt(glm::dot(vec, vec)) * mag;
}

Vector truncate(const Vector vec, const float max) {
    const float mag_sq = glm::dot(vec, vec);
    const float max_mag_sq = glm::dot(max, max);
    const float scale = mag_sq > max_mag_sq ? fastInverseSqrt(mag_sq / max_mag_sq) : 1.0f;
    return vec * scale;
}
