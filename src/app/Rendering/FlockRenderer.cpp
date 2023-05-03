#include "pch.hpp"
#include "FlockRenderer.hpp"


FlockRenderer::FlockRenderer(Unsigned initial_count, const Transform &projection) :
        m_data_buffer_size(static_cast<GLsizeiptr>(initial_count * sizeof(Transform))),
        m_data_buffer(std::make_unique<Transform[]>(m_data_buffer_size))
{
    m_layout.array(m_model, 0, 0, 3 * sizeof(Vector::value_type));
    m_layout.array(m_data, 1, 0, sizeof(Transform));
    m_layout.instances = initial_count;

    m_layout.attribute(0, 0, Vector::length(), lwvl::ByteFormat::Float, 0);
    m_layout.attribute(1, 1, Transform::length(), lwvl::ByteFormat::Float, 0);
    m_layout.attribute(1, 2, Transform::length(), lwvl::ByteFormat::Float, 4 * sizeof(Transform::value_type));
    m_layout.attribute(1, 3, Transform::length(), lwvl::ByteFormat::Float, 8 * sizeof(Transform::value_type));
    m_layout.attribute(1, 4, Transform::length(), lwvl::ByteFormat::Float, 12 * sizeof(Transform::value_type));
    m_layout.divisor(0, 0);
    m_layout.divisor(1, 1);
    m_layout.divisor(2, 1);
    m_layout.divisor(3, 1);
    m_layout.divisor(4, 1);

    m_data.store<Transform>(nullptr, m_data_buffer_size, lwvl::bits::Dynamic | lwvl::bits::Client);
    m_model.store(BoidShape.begin(), BoidShape.end());

    lwvl::VertexShader vs {lwvl::VertexShader::fromFile("Data/Shaders/boid.vertex")};
    lwvl::FragmentShader fs {lwvl::FragmentShader::fromFile("Data/Shaders/boid.fragment")};
    m_control.link(vs, fs);
    m_control.bind();

    m_control.uniform("projection").matrix4F(&projection[0][0]);
    u_view = m_control.uniform("view");
    u_color = m_control.uniform("color");
    u_color.setF(Boid::Color.r, Boid::Color.g, Boid::Color.b);
    m_control.bind();
}

void FlockRenderer::resize(Unsigned new_size) {
    m_layout.instances = new_size;
    auto buffer_size = static_cast<GLsizeiptr>(new_size * sizeof(Transform));
    if (buffer_size <= m_data_buffer_size) {
        return;
    }

    m_data_buffer_size = buffer_size;
    m_data_buffer = std::make_unique<Transform[]>(buffer_size);
    m_data = lwvl::Buffer();
    m_data.store<Transform>(nullptr, m_data_buffer_size, lwvl::bits::Dynamic | lwvl::bits::Client);
    m_layout.array(m_data, 1, 0, sizeof(Transform));
}

void FlockRenderer::update(const Flock &flock) {
    const Unsigned count = flock.count();
    const Boid *boids = flock.boids();
    const Quaternion *rotations = flock.rotations();
    auto buffer_size = static_cast<GLsizeiptr>(count * sizeof(Transform));

    constexpr Vector scale_vector = Vector{Boid::Scale};
    constexpr Transform unit {1.0f};
    for (Unsigned i = 0; i < count; ++i) {
        m_data_buffer[i] = (
            glm::translate(unit, boids[i].position)
            * glm::toMat4(rotations[i])
            * glm::scale(unit, scale_vector)
        );

        //Boid boid {boids[i]};
        //Vector rotation {glm::fastNormalize(boid.velocity)};
        //m_data_buffer[i] = Transform {
        //    Boid::Scale * rotation.x, Boid::Scale * rotation.y, 0.0f, 0.0f,
        //    -Boid::Scale * rotation.y, Boid::Scale * rotation.x, 0.0f, 0.0f,
        //    0.0f, 0.0f, 1.0f, 0.0f,
        //    boid.position.x, boid.position.y, boid.position.z, 1.0f
        //};
    }

    m_data.update<Transform>(m_data_buffer.get(), buffer_size, 0);
    m_layout.instances = count;
}

void FlockRenderer::set_view(const Transform &view) {
    GLint previous_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
    m_control.bind();
    u_view.matrix4F(&view[0][0]);
    glUseProgram(previous_program);
}

void FlockRenderer::set_color(const Triple &color) {
    m_control.bind();
    u_color.setF(color.r, color.g, color.b);
}

void FlockRenderer::draw() {
    m_control.bind();
    //m_layout.drawArrays(lwvl::PrimitiveMode::TriangleFan, 4);
    //m_layout.drawArrays(lwvl::PrimitiveMode::Triangles, 36);
    m_layout.drawArrays(lwvl::PrimitiveMode::Triangles, 18);
}
