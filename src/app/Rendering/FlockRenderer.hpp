#pragma once

#include "pch.hpp"
#include "Boid/Flock.hpp"


// Model is an array on the CPU that stores shape data
// Renderer takes the model and loads its shape data into GPU buffer
//constexpr std::array<float, 12> BoidShape {
//    +1.0f,      +0.0f,      +0.0f,
//    -0.707107f, +0.707107f, +0.0f,
//    -0.5f,      +0.0f,      +0.0f,
//    -0.707107f, -0.707107f, +0.0f
//};

//constexpr std::array<float, 54> BoidShape {
//     // Left Face
//    -1.0f, -1.0f, -1.0f,
//    -1.0f, -1.0f, +1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, -1.0f, -1.0f,
//
//    // Front Triangle
//    -1.0f, -1.0f, +1.0f,
//    +1.0f, +0.0f, +0.0f,
//    -1.0f, +1.0f, +1.0f,
//
//    // Back Triangle
//    -1.0f, -1.0f, -1.0f,
//    +1.0f, +0.0f, +0.0f,
//    -1.0f, +1.0f, -1.0f,
//
//    // Bottom Triangle
//    -1.0f, -1.0f, -1.0f,
//    +1.0f, +0.0f, +0.0f,
//    -1.0f, -1.0f, +1.0f,
//
//    // Top Triangle
//    -1.0f, +1.0f, +1.0f,
//    +1.0f, +0.0f, +0.0f,
//    -1.0f, +1.0f, -1.0f
//};

constexpr std::array<float, 54> BoidShape {
     // Left Face
    -1.0f, -0.5f, -0.5f,
    -1.0f, -0.5f, +0.5f,
    -1.0f, +0.5f, -0.5f,
    -1.0f, +0.5f, -0.5f,
    -1.0f, +0.5f, -0.5f,
    -1.0f, -0.5f, -0.5f,

    // Front Triangle
    -1.0f, -0.5f, +0.5f,
    +1.0f, +0.0f, +0.0f,
    -1.0f, +0.5f, +0.5f,

    // Back Triangle
    -1.0f, -0.5f, -0.5f,
    +1.0f, +0.0f, +0.0f,
    -1.0f, +0.5f, -0.5f,

    // Bottom Triangle
    -1.0f, -0.5f, -0.5f,
    +1.0f, +0.0f, +0.0f,
    -1.0f, -0.5f, +0.5f,

    // Top Triangle
    -1.0f, +0.5f, +0.5f,
    +1.0f, +0.0f, +0.0f,
    -1.0f, +0.5f, -0.5f
};

//constexpr std::array<float, 108> BoidShape {
//    // Front Face
//    -1.0f, -1.0f, +1.0f,
//    +1.0f, -1.0f, +1.0f,
//    +1.0f, +1.0f, +1.0f,
//    +1.0f, +1.0f, +1.0f,
//    -1.0f, +1.0f, +1.0f,
//    -1.0f, -1.0f, +1.0f,
//
//    // Right Face
//    +1.0f, -1.0f, +1.0f,
//    +1.0f, -1.0f, -1.0f,
//    +1.0f, +1.0f, -1.0f,
//    +1.0f, +1.0f, -1.0f,
//    +1.0f, +1.0f, +1.0f,
//    +1.0f, -1.0f, +1.0f,
//
//    // Back Face
//    +1.0f, -1.0f, -1.0f,
//    -1.0f, -1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    +1.0f, +1.0f, -1.0f,
//    +1.0f, -1.0f, -1.0f,
//
//    // Left Face
//    -1.0f, -1.0f, -1.0f,
//    -1.0f, -1.0f, +1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, -1.0f, -1.0f,
//
//    // Bottom Face
//    -1.0f, -1.0f, -1.0f,
//    +1.0f, -1.0f, -1.0f,
//    +1.0f, -1.0f, +1.0f,
//    +1.0f, -1.0f, +1.0f,
//    -1.0f, -1.0f, +1.0f,
//    -1.0f, -1.0f, -1.0f,
//
//    // Top Face
//    -1.0f, +1.0f, +1.0f,
//    +1.0f, +1.0f, +1.0f,
//    +1.0f, +1.0f, -1.0f,
//    +1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, -1.0f,
//    -1.0f, +1.0f, +1.0f,
//};


class FlockRenderer {
public:
    FlockRenderer(Unsigned initial_count, const Transform &projection);
    void resize(Unsigned new_size);
    void update(const Flock &flock);
    void set_view(const Transform &view);
    void set_color(const Triple &color);
    void draw();
private:
    lwvl::Program m_control;
    lwvl::VertexArray m_layout;
    lwvl::Buffer m_model;
    lwvl::Buffer m_data;
    lwvl::Uniform u_view;
    lwvl::Uniform u_color;
    GLsizeiptr m_data_buffer_size {0};
    std::unique_ptr<Transform[]> m_data_buffer;
};
