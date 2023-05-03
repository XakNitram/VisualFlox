#pragma once

#include "pch.hpp"
#include "Math/Math.hpp"


struct Boid {
    static constexpr float Scale = 1.0f;
    static constexpr float MaxSpeed = 10.0f;
    static constexpr float MaxForce = 0.3f;

    static constexpr float SeparationRadius = 2.1f * Scale * 1.4142135623730951f;  // sqrt(2)
    static constexpr float NeighborRadius = 2.0f * SeparationRadius;

    static constexpr float AlignmentWeight = 1.0f;
    static constexpr float CohesionWeight = 1.0f;
    static constexpr float SeparationWeight = 0.5f;
    static constexpr float PrimadonnaWeight = 3.0f;
    static constexpr float MaintainSpeedWeight = 0.01f;

    static constexpr Vector Color {0.76471f, 0.04314f, 0.30588f};

    Vector position {0.0f, 0.0f, 0.0f};
    Vector velocity {0.0f, 0.0f, 0.0f};

    Boid() = default;
    Boid(Vector position, Vector velocity);
};
