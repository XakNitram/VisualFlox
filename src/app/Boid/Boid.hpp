#pragma once

#include "pch.hpp"
#include "Math/Math.hpp"


struct Boid {
    static constexpr float Scale = 10.0f;
    static constexpr float MaxSpeed = 100.0f;
    static constexpr float MaxForce = 3.0f;

    static constexpr float NeighborRadius = 30.0f;
    static constexpr float SeparationRadius = 15.0f;
    static constexpr float AlignmentWeight = 1.0f;
    static constexpr float CohesionWeight = 1.0f;
    static constexpr float SeparationWeight = 1.5f;

    Vector position {0.0f, 0.0f, 0.0f};
    Vector velocity {0.0f, 0.0f, 0.0f};
};
