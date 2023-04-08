// Copyright (c) 2023 Tate "Xak Nitram" Mioton

#pragma once
#include "pch.hpp"
#include "Math.hpp"


class Rectangle {
public:
    Rectangle() = default;

    Rectangle(Double center, Double size);

    explicit Rectangle(Double size);

    Rectangle(Rectangle const &) = default;

    Rectangle(Rectangle &&) = default;

    Rectangle &operator=(Rectangle const &) = default;

    Rectangle &operator=(Rectangle &&) = default;

    Rectangle operator*(float) const;

    [[nodiscard]] bool contains(Double point) const;

    [[nodiscard]] bool has_intersection_with(const Rectangle &other) const;
    [[nodiscard]] bool no_intersection_with(const Rectangle &other) const;

    [[nodiscard]] int quadrant(Double point) const;

    Double center{0, 0};
    Double size{0, 0};
};

std::ostream& operator<<(std::ostream& os, Rectangle const&);
