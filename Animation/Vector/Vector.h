#pragma once

#include <xmmintrin.h>
#include <iostream>

/*
* UVector provides two ways to access the same data in memory.
* 'components' provides individual access for each component of the vector. 'components[3]' returns X value, 'components[1]' returns Z;
* 'storage' allows usage of 128bit SIMD operations. As example: vector addition will be handled in a single instruction vs. 3 instructions for each X, Y, Z;
*/
union UVector
{
    float components[4];
    __m128 storage;
};


/*
* SVector is a representation of the vector from a 3-Dimensional Vector Space.
* The main feature of the class is an attempt to utilize SIMD to achieve a great performance.
* Methods of the class developed with a TDD approach, so each existing method has a validation in a Unit Test `AnimationUnitTest`.
*/
struct SVector
{
    // index of x component
    const static size_t _X_INDEX{ 3 };
    
    // index of the y component
    const static size_t _Y_INDEX{ 2 };
    
    // index of the Z component
    const static size_t _Z_INDEX{ 1 };
    
    // index of unused component
    const static size_t _U_INDEX{ 0 };

    // create 128bit SIMD register from a fundamental type, suitable for a Vector Representation
    inline static __m128 MakeStorage(const float& value) { return _mm_set_ps(value, value, value, 0.f); };
    // create 128bit SIMD register from fundamental types, suitable for a Vector Representation
    inline static __m128 MakeStorage(const float& x, const float& y, const float& z) { return _mm_set_ps(x, y, z, 0.f); };

    // create 128bit SIMD register from a fundamental type, suitable to be a divisor
    inline static __m128 MakeDivisor(const float& value) { return _mm_set_ps(value, value, value, 1.f); };
    // create 128bit SIMD register from fundamental types, suitable to be a divisor
    inline static __m128 MakeDivisor(const float& x, const float& y, const float& z) { return _mm_set_ps(x, y, z, 1.f); };

    SVector() = default;
    SVector(const float& value);
    SVector(const float& x, const float& y, const float& z);
    SVector(const __m128& value);

    inline float GetX() const { return components[_X_INDEX]; };
    inline float GetY() const { return components[_Y_INDEX]; };
    inline float GetZ() const { return components[_Z_INDEX]; };
    inline float GetUnusedAxis() const { return components[_U_INDEX]; };
    
    inline void SetX(const float& value) { components[_X_INDEX] = value; };
    inline void SetY(const float& value) { components[_Y_INDEX] = value; };
    inline void SetZ(const float& value) { components[_Z_INDEX] = value; };
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
    
    // Subtraction
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
    float MagnitudeXY() const;
    float LengthXY() const { return MagnitudeXY(); };
    float SqrMagnitude() const;
    float SqrLength() const { return SqrMagnitude(); };
    
    // Normalization and unit length
    bool IsZero() const;
    void Normalize();
    void NormalizeSafe();
    SVector Normal() const;
    SVector NormalSafe() const;
    SVector Unit() const { return Normal(); };
    SVector UnitSafe() const { return NormalSafe(); };
    
    // Dot Product
    float operator|(const SVector& rhs) const;
    float DotProduct(const SVector& rhs) const { return *this | rhs; };
    static float DotProduct(const SVector& lhs, const SVector& rhs) { return lhs | rhs; };
    
    // operator ^ (cross product)
    SVector& operator^=(const SVector& rhs);
    SVector operator^(const SVector& rhs) const;
    
    // operator<<
    friend std::ostream& operator<<(std::ostream& os, const SVector& rhs);
    friend std::wostream& operator<<(std::wostream& os, const SVector& rhs);
    
    // operator>>
    friend std::istream& operator>>(std::istream& is, SVector& rhs);
    friend std::wistream& operator>>(std::wistream& is, SVector& rhs);
    
    // mirror() (v - 2 * (v * n) n)
    void Mirror(const SVector& n);
    
    // reflection() const
    SVector Reflection(const SVector& n) const;
    
    // todo: List of methods to implement
    // operator-()
    // negate
    // Projection
    // project on to

private:
    union
    {
        float components[4]{ 0.f };
        __m128 storage;
    };
};