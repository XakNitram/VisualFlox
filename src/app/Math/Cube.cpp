#include "pch.hpp"
#include "Cube.hpp"


Cube::Cube(Triple c, Triple s) : center(c), size(s) {}

Cube::Cube(Triple s) : center(0.0f, 0.0f, 0.0f), size(s) {}

bool Cube::contains(Triple p) const {
    Triple min {center - size};
    Triple max {center + size};
    return !(
        p.x < min.x || p.x > max.x ||
        p.y < min.y || p.y > max.y ||
        p.z < min.z || p.z > max.z
    );
}

bool Cube::has_intersection_with(const Cube &other) const {
    return !(
        other.center.x + other.size.x < center.x - size.x ||
        other.center.x - other.size.x > center.x + size.x ||
        other.center.y + other.size.y < center.y - size.y ||
        other.center.y - other.size.y > center.y + size.y ||
        other.center.z + other.size.z < center.z - size.z ||
        other.center.z - other.size.z > center.z + size.z
    );
}

bool Cube::no_intersection_with(const Cube &other) const {
    return (
        other.center.x + other.size.x < center.x - size.x ||
        other.center.x - other.size.x > center.x + size.x ||
        other.center.y + other.size.y < center.y - size.y ||
        other.center.y - other.size.y > center.y + size.y ||
        other.center.z + other.size.z < center.z - size.z ||
        other.center.z - other.size.z > center.z + size.z
    );
}

Cube Cube::operator*(float val) const {
    return {center, size * val};
}

std::ostream &operator<<(std::ostream &os, const Cube &rhs) {
    os << "Cube(center: "
    << rhs.center.x << ", " << rhs.center.y << ", " << rhs.center.z
    << ", size: "
    << rhs.size.x << ", " << rhs.size.y << ", " << rhs.size.z
    << ")";
    return os;
}
