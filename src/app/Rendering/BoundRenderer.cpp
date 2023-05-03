#include "pch.hpp"
#include "BoundRenderer.hpp"


BoundRenderer::BoundRenderer(const Cube bounds, const Transform &projection) : m_bounds(bounds) {
    m_layout.array(m_vertices, 0, 0, 3 * sizeof(float));
    m_layout.attribute(0, 0, 3, lwvl::ByteFormat::Float, 0);
    m_vertices.store(CubeWireframeShape.begin(), CubeWireframeShape.end());

    lwvl::VertexShader vs {lwvl::VertexShader::fromFile("Data/Shaders/default.vertex")};
    lwvl::FragmentShader fs {lwvl::FragmentShader::fromFile("Data/Shaders/default.fragment")};
    m_control.link(vs, fs);
    m_control.bind();
    m_control.uniform("model").matrix4F(&glm::scale(Transform{1.0f}, m_bounds.size)[0][0]);
    m_control.uniform("projection").matrix4F(&projection[0][0]);
    m_control.uniform("color").setF(1.0f, 1.0f, 1.0f);
    u_view = m_control.uniform("view");
}

void BoundRenderer::set_view(const Transform &view) {
    GLint previous_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
    m_control.bind();
    u_view.matrix4F(&view[0][0]);
    glUseProgram(previous_program);
}

void BoundRenderer::draw() {
    m_control.bind();
    m_layout.drawArrays(lwvl::PrimitiveMode::Lines, 24);
}
