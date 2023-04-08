#pragma once

#include "pch.hpp"
#include "Boid/Flock.hpp"


// Model is an array on the CPU that stores shape data
// Renderer takes the model and loads its shape data into GPU buffer
constexpr std::array<float, 12> BoidShape {
    +1.0f,      +0.0f,      +0.0f,
    -0.707107f, +0.707107f, +0.0f,
    -0.5f,      +0.0f,      +0.0f,
    -0.707107f, -0.707107f, +0.0f
};

//constexpr std::array<uint32_t, 6> FilledIndices {
//    0, 1, 2,
//    2, 3, 0
//};


class FlockRenderer {
public:
    FlockRenderer(Unsigned initial_count, const Projection &projection);
    void resize(Unsigned new_size);
    void update(const Flock &flock);
    void draw();
private:
    lwvl::Program m_control;
    lwvl::VertexArray m_layout;
    lwvl::Buffer m_model;
    lwvl::Buffer m_data;
    GLsizeiptr m_data_buffer_size {0};
    std::unique_ptr<Transform[]> m_data_buffer;
};
