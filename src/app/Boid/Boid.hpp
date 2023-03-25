#pragma once
#include "pch.hpp"


using Vector = glm::vec3;

struct Boid {
    static constexpr float MaxSpeed = 100.0f;
    static constexpr float MaxForce = 3.0f;

    Vector position {0.0f, 0.0f, 0.0f};
    Vector velocity {0.0f, 0.0f, 0.0f};
};


inline float fastInverseSqrt(const float d) {
    constexpr std::uint32_t magic = 0x5f3759df;
    // reinterpret_cast doesn't appear to have too much effect compared to *(long*)&d
    const std::uint32_t i = magic - (*reinterpret_cast<std::uint32_t*>(const_cast<float*>(&d)) >> 1);
    const float y = *reinterpret_cast<float*>(const_cast<std::uint32_t*>(&i));
    return y * (1.5f - (d * 0.5f * y * y));
}

inline Vector magnitude(const Vector vec, const float mag) {
    return vec * fastInverseSqrt(glm::dot(vec, vec)) * mag;
}

//inline Vector truncate(const Vector vec, const float max) {
//    constexpr float Enable[2]  {0.0f, 1.0f};
//    constexpr float Disable[2] {1.0f, 0.0f};
//    const float i = max * fastInverseSqrt(glm::dot(vec, vec));
//    const bool do_truncate = i < 1.0f;
//    return vec * (i * Enable[do_truncate] + Disable[do_truncate]);
//}

//inline Vector steer(const Vector vec, const Vector velocity) {
//    return truncate(magnitude(vec, Boid::MaxSpeed) - velocity, Boid::MaxForce);
//}
