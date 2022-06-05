#pragma once

#include <xmmintrin.h>

struct SVector
{
    SVector(const float& value);
    SVector(const float& x, const float& y, const float& z);
    SVector(const __m128& value);

    float GetX() const { return components[3]; };
    float GetY() const { return components[2]; };
    float GetZ() const { return components[1]; };

    const static SVector ZeroVector;

    // Arithmetics
    // Addition
    SVector& operator+=(const SVector& rhs);
    friend SVector operator+(const SVector& lhs, const SVector& rhs);
    // Substraction
    SVector& operator-=(const SVector& rhs);
    friend SVector operator-(const SVector& lhs, const SVector& rhs);

private:
    union
    {
        float components[4];
        __m128 storage;
    };
};

