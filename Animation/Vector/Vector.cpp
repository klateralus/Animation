#include "Vector.h"

#include <pmmintrin.h>
#include <math.h>

const SVector SVector::ZeroVector{ 0.f };

SVector::SVector(const float& value)
{
    storage = MakeStorage(value);
}

SVector::SVector(const float& x, const float& y, const float& z)
{
    storage = MakeStorage(x, y, z);
}

SVector::SVector(const __m128& value)
    : storage{value}
{
    ResetUnusedAxis();
}

/*            Equality            */
bool SVector::operator==(const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpeq_ps(storage, rhs.storage);
    return result.components[0] && result.components[1] && result.components[2] && result.components[3];
}

bool operator==(const SVector& lhs, const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpeq_ps(lhs.storage, rhs.storage);
    return result.components[0] && result.components[1] && result.components[2] && result.components[3];
}

/*            Inequality            */
bool SVector::operator!=(const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpneq_ps(storage, rhs.storage);
    return result.components[3] || result.components[2] || result.components[1] || result.components[0];
}

bool operator!=(const SVector& lhs, const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpneq_ps(lhs.storage, rhs.storage);
    return result.components[3] || result.components[2] || result.components[1] || result.components[0];
}

/*            Addition            */
SVector& SVector::operator+=(const SVector& rhs)
{
    storage = _mm_add_ps(storage, rhs.storage);
    return *this;
}

SVector operator+(const SVector& lhs, const SVector& rhs)
{
    return SVector(_mm_add_ps(lhs.storage, rhs.storage));
}

SVector& SVector::operator+=(const float& value)
{
    storage = _mm_add_ps(storage, MakeStorage(value));
    return *this;
}

SVector operator+(const SVector& vec, const float& value)
{
    return SVector(_mm_add_ps(vec.storage, SVector::MakeStorage(value)));
}

SVector operator+(const float& value, const SVector& vec)
{
    return SVector(_mm_add_ps(vec.storage, SVector::MakeStorage(value)));
}

/*            Substraction            */
SVector& SVector::operator-=(const SVector& rhs)
{
    storage = _mm_sub_ps(storage, rhs.storage);
    return *this;
}

SVector operator-(const SVector& lhs, const SVector& rhs)
{
    return SVector(_mm_sub_ps(lhs.storage, rhs.storage));
}

SVector& SVector::operator-=(const float& value)
{
    storage = _mm_sub_ps(storage, MakeStorage(value));
    return *this;
}

SVector operator-(const SVector& vec, const float& value)
{
    return SVector(_mm_sub_ps(vec.storage, SVector::MakeStorage(value)));
}

SVector operator-(const float& value, const SVector& vec)
{
    return SVector(_mm_sub_ps(SVector::MakeStorage(value), vec.storage));
}

/*            Multiplication            */
SVector& SVector::operator*=(const SVector& rhs)
{
    storage = _mm_mul_ps(storage, rhs.storage);
    return *this;
}

SVector operator*(const SVector& lhs, const SVector& rhs)
{
    return SVector(_mm_mul_ps(lhs.storage, rhs.storage));
}

SVector& SVector::operator*=(const float& value)
{
    storage = _mm_mul_ps(storage, MakeStorage(value));
    return *this;
}

SVector operator*(const SVector& vec, const float& value)
{
    return SVector(_mm_mul_ps(vec.storage, SVector::MakeStorage(value)));
}

SVector operator*(const float& value, const SVector& vec)
{
    return SVector(_mm_mul_ps(vec.storage, SVector::MakeStorage(value)));
}

/*            Division            */
SVector& SVector::operator/=(const SVector& rhs)
{
    storage = _mm_div_ps(storage, rhs.storage);
    components[_U_INDEX] = 0.f;
    return *this;
}

SVector operator/(const SVector& lhs, const SVector& rhs)
{
    SVector result(_mm_div_ps(lhs.storage, rhs.storage));
    result.components[SVector::_U_INDEX] = 0.f;
    return result;
}

SVector& SVector::operator/=(const float& value)
{
    storage = _mm_div_ps(storage, MakeStorage(value));
    components[_U_INDEX] = 0.f;
    return *this;
}

SVector operator/(const SVector& vec, const float& value)
{
    SVector result(_mm_div_ps(vec.storage, SVector::MakeStorage(value)));
    result.components[SVector::_U_INDEX] = 0.f;
    return result;
}

SVector operator/(const float& value, const SVector& vec)
{
    SVector result(_mm_div_ps(SVector::MakeStorage(value), vec.storage));
    result.components[SVector::_U_INDEX] = 0.f;
    return result;
}

float SVector::Magnitude() const
{
    UVector v;
    __m128 sqrs = _mm_mul_ps(storage, storage);
    // let's calcualte the sum of the squares
    __m128 sums = _mm_hadd_ps(sqrs, sqrs);      // {0.0f + z, y + x, 0.0f + z, y + x }
    sums = _mm_hadd_ps(sqrs, sqrs);             // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}
    // let's get the square root only for one component
    v.storage = _mm_sqrt_ss(sums);
    return v.components[0];
}

float SVector::LengthXY() const
{
    UVector v;
    __m128 sqrs = _mm_mul_ps(storage, storage);
    v.storage = _mm_hadd_ps(sqrs, sqrs);      // {0.0f + z, y + x, 0.0f + z, y + x }
    return sqrtf(v.components[1]);
}

float SVector::SqrMagnitude() const
{
    UVector v;
    __m128 sqrs = _mm_mul_ps(storage, storage);
    __m128 sums = _mm_hadd_ps(sqrs, sqrs);      // {0.0f + z, y + x, 0.0f + z, y + x }
    v.storage = _mm_hadd_ps(sqrs, sqrs);        // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}
    return v.components[0];
}