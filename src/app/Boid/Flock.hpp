#pragma once

#include "Algorithm/Algorithm.hpp"
#include "Structures/DoubleBuffer.hpp"
#include "Boid.hpp"


class Flock {
    // without any steering, this number can go above 500,000 before dipping below 60fps
    size_t m_count;
    DoubleBuffer<Boid> m_flock;
    std::vector<Quaternion> m_rotations;

public:
    explicit Flock(size_t flock_size);

    void update(Algorithm *, float dt);

    [[nodiscard]] const Boid *boids() const;
    [[nodiscard]] const Quaternion *rotations() const;
    [[nodiscard]] std::size_t count() const;
};
