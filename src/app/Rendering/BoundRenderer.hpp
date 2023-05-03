#pragma once
#include "pch.hpp"
#include "Math/Cube.hpp"


constexpr std::array<float, 72> CubeWireframeShape {
    // Front Face
    -1.0f, -1.0f, +1.0f,
    +1.0f, -1.0f, +1.0f,
    +1.0f, -1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,
    +1.0f, +1.0f, +1.0f,
    -1.0f, +1.0f, +1.0f,
    -1.0f, +1.0f, +1.0f,
    -1.0f, -1.0f, +1.0f,

    // Back Face
    +1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, +1.0f, -1.0f,
    -1.0f, +1.0f, -1.0f,
    +1.0f, +1.0f, -1.0f,
    +1.0f, +1.0f, -1.0f,
    +1.0f, -1.0f, -1.0f,

    // Left Face
    -1.0f, +1.0f, -1.0f,
    -1.0f, +1.0f, +1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, +1.0f,

    // Right Face
    +1.0f, +1.0f, -1.0f,
    +1.0f, +1.0f, +1.0f,
    +1.0f, -1.0f, -1.0f,
    +1.0f, -1.0f, +1.0f
};

class BoundRenderer {
public:
    BoundRenderer(Cube bounds, const Transform &projection);
    void set_view(const Transform &view);
    void draw();
private:
    lwvl::VertexArray m_layout;
    lwvl::Program m_control;
    lwvl::Buffer m_vertices;
    lwvl::Uniform u_view;
    Cube m_bounds;
};
