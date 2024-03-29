#pragma once
#include "pch.hpp"
#include "Algorithm.hpp"


class DirectLoopAlgorithm : public Algorithm {
public:
    explicit DirectLoopAlgorithm(Vector bounds);
    void update(DoubleBuffer<Boid> &boids, std::vector<Quaternion> &rotations, float delta) override;

private:
    Vector m_bounds;
};
