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

    UVector()
        : storage{_mm_setzero_ps()}
    {}
};


/*
* SVector is a representation of the vector from a 3-Dimensional Vector Space.
* The main feature of the class is an attempt to utilize SIMD to achieve a great performance.
* Methods of the class developed with a TDD approach, so each existing method has a validation in a Unit Test `AnimationUnitTest`.
*/
struct SVector
{
    // index of x component
    constexpr static size_t X_INDEX{ 3 };
    
    // index of the y component
    constexpr static size_t Y_INDEX{ 2 };
    
    // index of the Z component
    constexpr static size_t Z_INDEX{ 1 };
    
    // index of unused component
    constexpr static size_t U_INDEX{ 0 };

    // create 128bit SIMD register from a fundamental type, suitable for a Vector Representation
    static __m128 MakeStorage(const float& value) { return _mm_set_ps(value, value, value, 0.f); }
    // create 128bit SIMD register from fundamental types, suitable for a Vector Representation
    static __m128 MakeStorage(const float& x, const float& y, const float& z) { return _mm_set_ps(x, y, z, 0.f); }

    // create 128bit SIMD register from a fundamental type, suitable to be a divisor
    static __m128 MakeDivisor(const float& value) { return _mm_set_ps(value, value, value, 1.f); }
    // create 128bit SIMD register from fundamental types, suitable to be a divisor
    static __m128 MakeDivisor(const float& x, const float& y, const float& z) { return _mm_set_ps(x, y, z, 1.f); }

    SVector();
    SVector(const float& value);
    SVector(const float& x, const float& y, const float& z);
    SVector(const __m128& value);

    float GetX() const { return components[X_INDEX]; }
    float GetY() const { return components[Y_INDEX]; }
    float GetZ() const { return components[Z_INDEX]; }
    float GetUnusedAxis() const { return components[U_INDEX]; }
    
    void SetX(const float& value) { components[X_INDEX] = value; }
    void SetY(const float& value) { components[Y_INDEX] = value; }
    void SetZ(const float& value) { components[Z_INDEX] = value; }
    void SetUnusedAxis(const float& value) { components[U_INDEX] = value; }
    
    void ResetUnusedAxis() { components[U_INDEX] = 0.f; }

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
    float Length() const { return Magnitude(); }
    float MagnitudeXY() const;
    float LengthXY() const { return MagnitudeXY(); }
    float SqrMagnitude() const;
    float SqrLength() const { return SqrMagnitude(); }
    
    // Normalization and unit length
    bool IsZero() const;
    void Normalize();
    void NormalizeSafe();
    SVector Normal() const;
    SVector NormalSafe() const;
    SVector Unit() const { return Normal(); }
    SVector UnitSafe() const { return NormalSafe(); }
    
    // Dot Product
    float operator|(const SVector& rhs) const;
    float DotProduct(const SVector& rhs) const { return *this | rhs; }
    static float DotProduct(const SVector& lhs, const SVector& rhs) { return lhs | rhs; }
    
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

    // negation
    SVector operator-() const;
    void Negate();

    // projection
    /* When we project vector on a non-unit vector we need to normalize dot product */
    void ProjectOnTo(const SVector& v);
    SVector ProjectionOnTo(const SVector& v) const;
    
    /* When we project vector on an unit vector, we can drop the normalization part */
    void ProjectOnToNormal(const SVector& n);
    SVector ProjectionOnToNormal(const SVector& n) const;

    // rejection
    /* When we find a rejection vector to a non-unit vector we need to normalize dot product */
    void RejectTo(const SVector& v);
    SVector RejectionTo(const SVector& v) const;

    void RejectToNormal(const SVector& n);
    SVector RejectionToNormal(const SVector& n) const;
    
    // todo: List of methods to implement
    // outer product

private:
    union
    {
        float components[4];
        __m128 storage{_mm_setzero_ps()};
    };
};