#pragma once

#include <cstdint>
#include <numeric>


template<std::uint8_t P>
class Fixed {
public:
    using UnderlyingType = std::int32_t;
    using LargerType = std::int64_t;
    using RealType = double;

    static constexpr auto Precision = P;
    //static constexpr auto Factor = static_cast<RealType>(1 << Precision);
    static constexpr auto Factor = static_cast<UnderlyingType>(1) << Precision;

    static_assert(
        std::numeric_limits<UnderlyingType>::digits >= Precision,
        "Precision exceeds available storage bits"
    );

    Fixed() = default;

    template<typename T>
    explicit Fixed(T value)
        : m_value(static_cast<UnderlyingType>(value * Factor)) {}

    template<std::uint8_t OtherPrecision>
    explicit Fixed(Fixed<OtherPrecision> other)
        : m_value(
        static_cast<UnderlyingType>(other.m_value *
                                    Factor / other.Factor)) {}

    Fixed operator-() const {
        Fixed result;
        result.m_value = -m_value;
        return result;
    }

    Fixed &operator+=(Fixed other) {
        m_value += other.m_value;
        return *this;
    }

    Fixed &operator-=(Fixed other) {
        m_value -= other.m_value;
        return *this;
    }

    Fixed &operator*=(Fixed other) {
        m_value = static_cast<UnderlyingType>(
            //(static_cast<RealType>(m_value) * other.m_value) / Factor
            (static_cast<LargerType>(m_value) * other.m_value) / Factor
        );
        return *this;
    }

    Fixed &operator/=(Fixed other) {
        m_value = static_cast<UnderlyingType>(
            //(static_cast<RealType>(m_value) * Factor) / other.m_value
            (static_cast<LargerType>(m_value) * Factor) / other.m_value
        );
        return *this;
    }

    friend Fixed operator+(Fixed lhs, Fixed rhs) {
        return lhs += rhs;
    }

    friend Fixed operator-(Fixed lhs, Fixed rhs) {
        return lhs -= rhs;
    }

    friend Fixed operator*(Fixed lhs, Fixed rhs) {
        return lhs *= rhs;
    }

    friend Fixed operator/(Fixed lhs, Fixed rhs) {
        return lhs /= rhs;
    }

    [[nodiscard]] UnderlyingType raw_value() const { return m_value; }

    [[nodiscard]] RealType real() const { return static_cast<RealType>(m_value) / Factor; }

private:
    UnderlyingType m_value;
};


/*
template <std::uint8_t Precision>
class FixedPoint {
public:
    using underlying_type = std::int32_t;
    static constexpr underlying_type scale_factor = static_cast<underlying_type>(1) << Precision;

    FixedPoint() : value(0) {}
    FixedPoint(double d) : value(static_cast<underlying_type>(d * scale_factor)) {}
    FixedPoint(underlying_type v) : value(v) {}

    operator double() const { return static_cast<double>(value) / scale_factor; }

    FixedPoint operator-() const { return FixedPoint(-value); }
    FixedPoint operator+(const FixedPoint& other) const { return FixedPoint(value + other.value); }
    FixedPoint operator-(const FixedPoint& other) const { return FixedPoint(value - other.value); }
    FixedPoint operator*(const FixedPoint& other) const { return FixedPoint((static_cast<std::int64_t>(value) * other.value) / scale_factor); }
    FixedPoint operator/(const FixedPoint& other) const { return FixedPoint((static_cast<std::int64_t>(value) * scale_factor) / other.value); }

    bool operator==(const FixedPoint& other) const { return value == other.value; }
    bool operator!=(const FixedPoint& other) const { return value != other.value; }
    bool operator<(const FixedPoint& other) const { return value < other.value; }
    bool operator<=(const FixedPoint& other) const { return value <= other.value; }
    bool operator>(const FixedPoint& other) const { return value > other.value; }
    bool operator>=(const FixedPoint& other) const { return value >= other.value; }

    FixedPoint& operator+=(const FixedPoint& other) { value += other.value; return *this; }
    FixedPoint& operator-=(const FixedPoint& other) { value -= other.value; return *this; }
...

#include <cstdint>
#include <limits>

template <std::uint8_t Precision>
class FixedPoint {
public:
    using underlying_type = std::int32_t;
    using real_type = double;

    static constexpr auto precision = Precision;
    static constexpr auto factor = static_cast<real_type>(1 << precision);

    static_assert(std::numeric_limits<underlying_type>::digits >= precision,
                  "Precision exceeds available storage bits");

    FixedPoint() = default;

    template <typename T>
    explicit FixedPoint(T value)
        : value_(static_cast<underlying_type>(value * factor))
    {}

    template <std::uint8_t OtherPrecision>
    explicit FixedPoint(FixedPoint<OtherPrecision> other)
        : value_(static_cast<underlying_type>(other.value_ *
                                              factor / other.factor))
    {}

    FixedPoint operator-() const
    {
        FixedPoint result;
        result.value_ = -value_;
        return result;
    }

    FixedPoint& operator+=(FixedPoint other)
    {
        value_ += other.value_;
        return *this;
    }

    FixedPoint& operator-=(FixedPoint other)
    {
        value_ -= other.value_;
        return *this;
    }

    FixedPoint& operator*=(FixedPoint other)
    {
        value_ = static_cast<underlying_type>(
            (static_cast<real_type>(value_) * other.value_) / factor);
        return *this;
    }

    FixedPoint& operator/=(FixedPoint other)
    {
        value_ = static_cast<underlying_type>(
            (static_cast<real_type>(value_) * factor) / other.value_);
        return *this;
    }

    friend FixedPoint operator+(FixedPoint lhs, FixedPoint rhs)
    {
        return lhs += rhs;
    }

    friend FixedPoint operator-(FixedPoint lhs, FixedPoint rhs)
    {
        return lhs -= rhs;
    }

    friend FixedPoint operator*(FixedPoint lhs, FixedPoint rhs)
    {
        return lhs *= rhs;
    }

    friend FixedPoint operator/(FixedPoint lhs, FixedPoint rhs)
    {
        return lhs /= rhs;
    }

    underlying_type raw_value() const { return value_; }

    real_type real() const { return static_cast<real_type>(value_) / factor; }

private:
    underlying_type value_

 */
