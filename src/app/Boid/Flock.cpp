#include "pch.hpp"
#include "Flock.hpp"


Flock::Flock(size_t flock_size) : m_count(flock_size), m_flock(flock_size), m_rotations(flock_size) {
    // Set up boid starting locations
    Boid *writable = m_flock.write();

    float d = 1.0f;
    float angle = 0.0f;
    for (ptrdiff_t i = 0; i < m_count; i++) {
        Boid boid;
        //auto angle = static_cast<float>(i) * tauOverSize;
        const float radius = glm::sqrt(static_cast<float>(i + 1));
        angle += glm::asin(1.0f / radius);

        const Vector offsets{glm::cos(angle) * radius * d, glm::sin(angle) * radius * d, 0.0f};
        boid.position = offsets;
        boid.velocity = magnitude(offsets + angle, Boid::MaxSpeed);

        writable[i] = boid;
        m_rotations[i] = glm::quat{1.0f, glm::normalize(boid.velocity)};
    }

    m_flock.flip();
}

void Flock::update(Algorithm *algorithm, float dt) {
    // Run the given algorithm
    algorithm->update(m_flock, m_rotations, dt);

    // Push changes to flock
    m_flock.flip();
}

const Boid *Flock::boids() const {
    return m_flock.read();
}

const Quaternion *Flock::rotations() const {
    return m_rotations.data();
}

std::size_t Flock::count() const {
    return m_count;
}
