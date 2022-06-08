#pragma once

#include <xmmintrin.h>

union UVector
{
    float components[4];
    __m128 storage;
};

struct SVector
{
    // index of the X axis
    const static size_t _X_INDEX{ 3 };
    // index of the Y axis
    const static size_t _Y_INDEX{ 2 };
    // index of the Z axis
    const static size_t _Z_INDEX{ 1 };
    // index of unused Axis
    const static size_t _U_INDEX{ 0 };

    inline static __m128 MakeStorage(const float& value) { return _mm_set_ps(value, value, value, 0.f); };
    inline static __m128 MakeStorage(const float& x, const float& y, const float& z) { return _mm_set_ps(x, y, z, 0.f); };

    SVector(const float& value);
    SVector(const float& x, const float& y, const float& z);
    SVector(const __m128& value);

    inline float GetX() const { return components[_X_INDEX]; };
    inline float GetY() const { return components[_Y_INDEX]; };
    inline float GetZ() const { return components[_Z_INDEX]; };
    inline float GetUnusedAxis() const { return components[_U_INDEX]; };
    inline void SetX(const float& value) { components[_X_INDEX] = value; };
    inline void SetY(const float& value) { components[_Y_INDEX] = value; };
    inline void SetUnusedAxis(const float& value) { components[_U_INDEX] = value; };
    inline void ResetUnusedAxis() { components[_U_INDEX] = 0.f; };

    const static SVector ZeroVector;
    // Equality
    bool operator==(const SVector& rhs);
    friend bool operator==(const SVector& lhs, const SVector& rhs);
    // Inequality
    bool operator!=(const SVector& rhs);
    friend bool operator!=(const SVector& lhs, const SVector& rhs);
    // Addition
    SVector& operator+=(const SVector& rhs);
    friend SVector operator+(const SVector& lhs, const SVector& rhs);
    SVector& operator+=(const float& value);
    friend SVector operator+(const SVector& vec, const float& value);
    friend SVector operator+(const float& value, const SVector& vec);
    // Substraction
    SVector& operator-=(const SVector& rhs);
    friend SVector operator-(const SVector& lhs, const SVector& rhs);
    SVector& operator-=(const float& value);
    friend SVector operator-(const SVector& vec, const float& value);
    friend SVector operator-(const float& value, const SVector& vec);
    // Multiplication
    SVector& operator*=(const SVector& rhs);
    friend SVector operator*(const SVector& lhs, const SVector& rhs);
    SVector& operator*=(const float& value);
    friend SVector operator*(const SVector& vec, const float& value);
    friend SVector operator*(const float& value, const SVector& vec);
    // Division
    SVector& operator/=(const SVector& rhs);
    friend SVector operator/(const SVector& lhs, const SVector& rhs);
    SVector& operator/=(const float& value);
    friend SVector operator/(const SVector& vec, const float& value);
    friend SVector operator/(const float& value, const SVector& vec);
    // Magnitude & Length
    float Magnitude() const;
    float Length() const { return Magnitude(); };
    float LengthXY() const;
    float SqrMagnitude() const;
    float SqrLength() const { return SqrMagnitude(); };
    // normal() const
    // normalize()
    // operator | (dot product)
    // operator ^ (cross product)
    // operator<<
    // operator>>

private:
    union
    {
        float components[4];
        __m128 storage;
    };
};