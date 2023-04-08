#include "pch.hpp"
#include "DirectLoopAlgorithm.hpp"
#include "Math/Cube.hpp"


DirectLoopAlgorithm::DirectLoopAlgorithm(Vector bounds) : m_bounds(bounds) {}

void DirectLoopAlgorithm::update(DoubleBuffer<Boid> &boids, float delta) {
    const std::size_t count = boids.count();
    const Boid *read = boids.read();
    Boid *write = boids.write();

    const Cube center_cube {m_bounds * 0.75f};
    for (int i = 0; i < count; ++i) {
        const Boid &current {read[i]};

        Vector center_steer {
            center_cube.contains(Triple(current.position)) ?
            Vector{0.0f, 0.0f, 0.0f} :
            glm::fastNormalize(
                glm::fastNormalize(-current.position) * Boid::MaxSpeed - current.velocity
            ) * (Boid::MaxForce)
        };

        const float ar2 = Boid::NeighborRadius * Boid::NeighborRadius;
        const float cr2 = ar2;
        const float sr2 = Boid::SeparationRadius * Boid::SeparationRadius;

        Vector alignment {0.0f, 0.0f, 0.0f};
        Vector cohesion {0.0f, 0.0f, 0.0f};
        Vector separation {0.0f, 0.0f, 0.0f};

        int alignment_total = 0;
        int cohesive_total = 0;
        int separation_total = 0;
        for (int j = 0; j < count; ++j) {
            if (i == j) { continue; }
            const Boid &other {read[j]};
            const float d2 = glm::distance2(current.position, other.position);
            if (d2 <= ar2) {
                alignment += other.velocity;
                ++alignment_total;
            }

            if (d2 <= cr2) {
                cohesion += other.position;
                ++cohesive_total;
            }

            if (d2 <= sr2) {
                separation += ((current.position - other.position) / (d2 + Epsilon));
                ++separation_total;
            }
        }

        if (alignment_total > 0) {
            alignment /= alignment_total;
            alignment = glm::fastNormalize(
                glm::fastNormalize(alignment) * Boid::MaxSpeed - current.velocity
            ) * Boid::MaxForce;
        }

        if (cohesive_total > 0) {
            cohesion /= cohesive_total;
            cohesion = glm::fastNormalize(
                glm::fastNormalize(cohesion) * Boid::MaxSpeed - current.velocity
            ) * Boid::MaxForce;
        }

        if (separation_total > 0) {
            separation /= separation_total;
            separation = glm::fastNormalize(
                glm::fastNormalize(separation) * Boid::MaxSpeed - current.velocity
            ) * Boid::MaxForce;
        }

        Vector acceleration {
            + alignment * Boid::AlignmentWeight
            + cohesion * Boid::CohesionWeight
            + separation * Boid::SeparationWeight
            + center_steer * (Boid::SeparationWeight * 2.0f)
        };

        //write[i].velocity = truncate(
        //    current.velocity + acceleration,
        //    Boid::MaxSpeed
        //);
        write[i].velocity = truncate(current.velocity + truncate(acceleration, Boid::MaxForce), Boid::MaxSpeed);
        write[i].position += write[i].velocity * delta;
    }
}
