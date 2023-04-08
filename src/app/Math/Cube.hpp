#pragma once

#include "pch.hpp"
#include "Math.hpp"


class Cube {
public:
    Cube() = default;
    Cube(Triple center, Triple size);
    explicit Cube(Triple size);
    Cube(const Cube &) = default;
    Cube(Cube &&) = default;

    Cube &operator=(const Cube &) = default;
    Cube &operator=(Cube&&) = default;
    Cube operator*(float) const;

    [[nodiscard]] bool contains(Triple point) const;

    [[nodiscard]] bool has_intersection_with(const Cube &other) const;
    [[nodiscard]] bool no_intersection_with(const Cube &other) const;

    //[[nodiscard]] int octant(Triple point) const;

    Triple center {0.0f, 0.0f, 0.0f};
    Triple size {0.0f, 0.0f, 0.0f};
};

std::ostream &operator<<(std::ostream &os, const Cube &rhs);
