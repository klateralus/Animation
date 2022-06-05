#include "Vector.h"

const SVector SVector::ZeroVector{ 0.f };

SVector::SVector(const float& value)
{
    storage = _mm_set_ps(value, value, value, 0.f);
}

SVector::SVector(const float& x, const float& y, const float& z)
{
    storage = _mm_set_ps(x, y, z, 0.f);
}

SVector::SVector(const __m128& value)
    : storage{value}
{
}

SVector& SVector::operator+=(const SVector& rhs)
{
    storage = _mm_add_ps(storage, rhs.storage);
    return *this;
}

SVector operator+(const SVector& lhs, const SVector& rhs)
{
    return SVector(_mm_add_ps(lhs.storage, rhs.storage));
}

SVector& SVector::operator-=(const SVector& rhs)
{
    storage = _mm_sub_ps(storage, rhs.storage);
    return *this;
}

SVector operator-(const SVector& lhs, const SVector& rhs)
{
    return SVector(_mm_sub_ps(lhs.storage, rhs.storage));
}