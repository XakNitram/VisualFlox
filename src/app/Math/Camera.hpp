// Copyright (c) 2023 Tate "Xak Nitram" Mioton

#pragma once

#include "pch.hpp"
#include "Math/Rectangle.hpp"


class Camera {
    using CameraMatrix = glm::mat<4, 4, float, glm::defaultp>;
    void apply_position();
public:
    Camera() = default;
    explicit Camera(Rectangle box, Double viewport);

    // Mutators
    void move(Double delta);
    void move_to(Double position);
    void zoom(float level);
    void zoom(Double zoom_vector);

    // Accessors
    [[nodiscard]] const float *data() const;
    [[nodiscard]] float zoom() const;
    [[nodiscard]] Double zoom_vector() const;
    [[nodiscard]] Double position() const;
    [[nodiscard]] Rectangle box() const;
private:
    CameraMatrix m_mat {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    Rectangle m_box {Double {1.0f, 1.0f}};
    Double m_viewport {1.0f, 1.0f};
    Double m_zoom {1.0f, 1.0f};
    float m_aspect = 1.0f;
};
