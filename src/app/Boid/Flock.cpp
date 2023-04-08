#include "pch.hpp"
#include "Flock.hpp"


Flock::Flock(size_t flock_size) : m_count(flock_size), m_flock(flock_size) {
    // Set up boid starting locations
    Boid *writable = m_flock.write();

    float d = 7.5f;
    float angle = 0.0f;
    for (ptrdiff_t i = 0; i < m_count; i++) {
        Boid &boid = writable[i];
        //auto angle = static_cast<float>(i) * tauOverSize;
        float radius = glm::sqrt(static_cast<float>(i + 1));
        angle += glm::asin(1.0f / radius);
        Vector offsets{glm::cos(angle) * radius * d, glm::sin(angle) * radius * d, 0.0f};
        boid.position = offsets;
        boid.velocity = magnitude(10.0f * offsets + angle, Boid::MaxSpeed);
    }

    m_flock.flip();
}

void Flock::update(Algorithm *algorithm, float dt) {
    // Run the given algorithm
    algorithm->update(m_flock, dt);

    // Push changes to flock
    m_flock.flip();
}

const Boid *Flock::boids() const {
    return m_flock.read();
}

std::size_t Flock::count() const {
    return m_count;
}
