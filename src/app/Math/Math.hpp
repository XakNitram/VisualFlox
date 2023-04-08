#pragma once

#include "pch.hpp"


using Double = glm::vec<2, float, glm::defaultp>;
using Triple = glm::vec<3, float, glm::defaultp>;
using Vector = glm::vec<3, float, glm::defaultp>;
using Quadruple = glm::vec<4, float, glm::defaultp>;
using Projection = glm::mat<4, 4, float, glm::defaultp>;
using Transform = glm::mat<4, 4, float, glm::defaultp>;
constexpr float Epsilon = std::numeric_limits<float>::epsilon();


float fastInverseSqrt(float d);

Vector magnitude(Vector vec, float mag);

Vector truncate(Vector vec, float max);

//inline Vector steer(const Vector vec, const Vector velocity) {
//    return truncate(magnitude(vec, Boid::MaxSpeed) - velocity, Boid::MaxForce);
//}

template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
std::ostream &operator<<(std::ostream &os, const glm::vec<L, T, Q> &rhs) {
    os.write("(", 1);
    for (glm::length_t i = 0; i < L; ++i) {
        if (i != 0) {
            os.write(", ", 2);
        }
        os << rhs[i];
    }
    os.write(")", 1);
    return os;
}
