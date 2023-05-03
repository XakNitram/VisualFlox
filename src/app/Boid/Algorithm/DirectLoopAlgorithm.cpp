#include "pch.hpp"
#include "DirectLoopAlgorithm.hpp"
#include "Math/Cube.hpp"


DirectLoopAlgorithm::DirectLoopAlgorithm(Vector bounds) : m_bounds(bounds) {}

void DirectLoopAlgorithm::update(DoubleBuffer<Boid> &boids, std::vector<Quaternion> &rotations, float delta) {
    const std::size_t count = boids.count();
    const Boid *read = boids.read();
    Boid *write = boids.write();

    const Cube center_cube {m_bounds * 0.75f};
    for (int i = 0; i < count; ++i) {
        const Boid &current {read[i]};  // Reference is faster in outer loop.

        Vector center_steer {
            center_cube.contains(Triple(current.position)) ?
            Vector {0.0f, 0.0f, 0.0f} :
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
            const Boid other {read[j]};  // Copy is faster in inner loop.
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

        const Vector full_speed = glm::fastNormalize(
            glm::fastNormalize(current.velocity) * Boid::MaxSpeed - current.velocity
        ) * Boid::MaxForce;

        const Vector acceleration {
            + alignment * Boid::AlignmentWeight
            + cohesion * Boid::CohesionWeight
            + separation * Boid::SeparationWeight
            + center_steer * Boid::PrimadonnaWeight
            + full_speed * Boid::MaintainSpeedWeight
        };

        const Vector new_velocity {current.velocity + truncate(acceleration, Boid::MaxForce)};
        const Vector new_position {current.position + (new_velocity * delta)};
        write[i] = Boid{new_position, new_velocity};
    }

    // Calculate rotation quaternions
    const Boid *rotation_read {write};
    for (int i = 0; i < count; ++i) {
        const Boid &current {rotation_read[i]};
        const Quaternion previous_quat {rotations[i]};
        const Vector basis {1.0f, 0.0f, 0.0f};
        const Vector normal_velocity = glm::normalize(current.velocity);

        const float angle = glm::acos(glm::dot(basis, normal_velocity));
        // This is the axis of rotation for the rotation from the origin to face the velocity vector.
        const Vector axis = glm::normalize(glm::cross(basis, normal_velocity));
        // We need a rotation that cancels out the roll component of the quaternion.
        // roll = atan2(2xy+2wz, 1 - 2y^2 - 2z^2)

        const Quaternion new_quat = glm::angleAxis(angle, axis);
        const float t = 6.0f * delta; // set a time step value between 0 and 1
        // interpolate smoothly between the previous and new quaternions
        const Quaternion target_quat = glm::slerp(previous_quat, new_quat, t);
        const Quaternion inverse_quat = glm::slerp(previous_quat, -new_quat, t);
        const float iq_alignment = glm::dot(new_quat, previous_quat);
        constexpr float eps = 1e-6f; // set a small threshold
        if (glm::length(new_quat - previous_quat) > eps) {
            rotations[i] = iq_alignment < 0.0f ? inverse_quat : target_quat;
        }
    }
}
