#include "Vector.h"

#include <pmmintrin.h>
#include <math.h>
#include <sstream>
#include <iomanip>

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
    return result.components[_X_INDEX] && result.components[_Y_INDEX] && result.components[_Z_INDEX] && result.components[_U_INDEX];
}

bool operator==(const SVector& lhs, const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpeq_ps(lhs.storage, rhs.storage);
    return result.components[SVector::_X_INDEX] && result.components[SVector::_Y_INDEX] && result.components[SVector::_Z_INDEX] && result.components[SVector::_U_INDEX];
}

/*            Inequality            */
bool SVector::operator!=(const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpneq_ps(storage, rhs.storage);
    return result.components[_X_INDEX] || result.components[_Y_INDEX] || result.components[_Z_INDEX] || result.components[_U_INDEX];
}

bool operator!=(const SVector& lhs, const SVector& rhs)
{
    UVector result;
    result.storage = _mm_cmpneq_ps(lhs.storage, rhs.storage);
    return result.components[SVector::_X_INDEX] || result.components[SVector::_Y_INDEX] || result.components[SVector::_Z_INDEX] || result.components[SVector::_U_INDEX];
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
    storage = _mm_div_ps(storage, MakeDivisor(value));
    return *this;
}

SVector operator/(const SVector& vec, const float& value)
{
    SVector result(_mm_div_ps(vec.storage, SVector::MakeDivisor(value)));
    return result;
}

SVector operator/(const float& value, const SVector& vec)
{
    SVector result(_mm_div_ps(SVector::MakeStorage(value), vec.storage));
    result.components[SVector::_U_INDEX] = 0.f;
    return result;
}

/*            Magnitude            */
float SVector::Magnitude() const
{
    UVector v;
    __m128 sqrs = _mm_mul_ps(storage, storage);
    // let's calcualte the sum of the squares
    __m128 sums = _mm_hadd_ps(sqrs, sqrs);      // {0.0f + z, y + x, 0.0f + z, y + x }
    sums = _mm_hadd_ps(sums, sums);             // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}
    // let's get the square root only for one component
    v.storage = _mm_sqrt_ss(sums);
    return v.components[0];
}

float SVector::MagnitudeXY() const
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
    v.storage = _mm_hadd_ps(sums, sums);        // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}
    return v.components[0];
}

/*            Normalization            */
bool SVector::IsZero() const
{
    UVector result;
    result.storage = _mm_cmpeq_ps(storage, ZeroVector.storage);
    return result.components[SVector::_X_INDEX] && result.components[SVector::_Y_INDEX] && result.components[SVector::_Z_INDEX];
}
void SVector::Normalize()
{
    const float length = Length();
    storage = _mm_div_ps(storage, MakeDivisor(length));
}

void SVector::NormalizeSafe()
{
    if (!IsZero())
    {
        Normalize();
    }
}

SVector SVector::Normal() const
{
    SVector result(*this);
    result.Normalize();
    return result;
}

SVector SVector::NormalSafe() const
{
    if (!IsZero())
    {
        return Normal();
    }
    return *this;
}

/*            Dot Product            */
float SVector::operator|(const SVector& rhs) const
{
    UVector v;
    v.storage = _mm_mul_ps(storage, rhs.storage);
    v.storage = _mm_hadd_ps(v.storage, v.storage);  // {0.0f + z, y + x, 0.0f + z, y + x }
    v.storage = _mm_hadd_ps(v.storage, v.storage);  // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}
    return v.components[_U_INDEX];
}

/*            Cross Product            */
SVector& SVector::operator^=(const SVector& rhs)
{
    __m128 a_yzx = _mm_shuffle_ps(storage, storage, _MM_SHUFFLE(_Y_INDEX, _Z_INDEX, _X_INDEX, _U_INDEX));
    //__m128 b_zxy = _mm_shuffle_ps(rhs.storage, rhs.storage, _MM_SHUFFLE(_Z_INDEX, _X_INDEX, _Y_INDEX, _U_INDEX));
    //__m128 a_zxy = _mm_shuffle_ps(storage, storage, _MM_SHUFFLE(_Z_INDEX, _X_INDEX, _Y_INDEX, _U_INDEX));
    __m128 b_yzx = _mm_shuffle_ps(rhs.storage, rhs.storage, _MM_SHUFFLE(_Y_INDEX, _Z_INDEX, _X_INDEX, _U_INDEX));
    //storage = _mm_sub_ps(_mm_mul_ps(a_yzx, b_zxy), _mm_mul_ps(a_zxy, b_yzx));
    __m128 c = _mm_sub_ps(_mm_mul_ps(storage, b_yzx), _mm_mul_ps(a_yzx, rhs.storage));
    storage = _mm_shuffle_ps(c, c, _MM_SHUFFLE(_Y_INDEX, _Z_INDEX, _X_INDEX, _U_INDEX));
    return *this;
}

SVector SVector::operator^(const SVector& rhs) const
{
    SVector result(*this);
    result ^= rhs;
    return result;
}

/*            Operator <<            */
ostream& operator<<(ostream& os, const SVector& rhs)
{
    os << fixed << setprecision(2) << "(" << rhs.GetX() << ", " << rhs.GetY() << ", " << rhs.GetZ() << ')';
    return os;
}

wostream& operator<<(wostream& os, const SVector& rhs)
{
    os << fixed << setprecision(2) << L"(" << rhs.GetX() << L", " << rhs.GetY() << L", " << rhs.GetZ() << L")";
    return os;
}

/*            Operator >>            */
istream& operator>>(istream& is, SVector& rhs)
{
    string input;
    getline(is, input);
    auto start_bracer = input.find("(");
    if (start_bracer == string::npos)
    {
        return is;
    }
    auto end_bracer = input.find(")");
    if (end_bracer == string::npos)
    {
        return is;
    }
    auto first_comma = input.find(",");
    if (first_comma == string::npos)
    {
        return is;
    }
    auto second_comma = input.find(",", first_comma + 1);
    if (second_comma == string::npos)
    {
        return is;
    }
    
    float value{ 0 };
    {
        istringstream x(input.substr(start_bracer + 1, first_comma - start_bracer - 1));
        x >> value;
        rhs.SetX(value);
    }
    
    {
        istringstream y(input.substr(first_comma + 1, second_comma - first_comma - 1));
        y >> value;
        rhs.SetY(value);
    }

    {
        istringstream z(input.substr(second_comma + 1, end_bracer - second_comma - 1));
        z >> value;
        rhs.SetZ(value);
    }

    return is;
}

wistream& operator>>(wistream& is, SVector& rhs)
{
    wstring input;
    getline(is, input);
    auto start_bracer = input.find(L"(");
    if (start_bracer == string::npos)
    {
        return is;
    }
    auto end_bracer = input.find(L")");
    if (end_bracer == string::npos)
    {
        return is;
    }
    auto first_comma = input.find(L",");
    if (first_comma == string::npos)
    {
        return is;
    }
    auto second_comma = input.find(L",", first_comma + 1);
    if (second_comma == string::npos)
    {
        return is;
    }

    float value{ 0 };
    {
        wistringstream x(input.substr(start_bracer + 1, first_comma - start_bracer - 1));
        x >> value;
        rhs.SetX(value);
    }

    {
        wistringstream y(input.substr(first_comma + 1, second_comma - first_comma - 1));
        y >> value;
        rhs.SetY(value);
    }

    {
        wistringstream z(input.substr(second_comma + 1, end_bracer - second_comma - 1));
        z >> value;
        rhs.SetZ(value);
    }

    return is;
}

/*            Reflection            */
void SVector::Mirror(const SVector& n)
{
    *this -= n * ( 2.f * (*this | n));
}

SVector SVector::Reflection(const SVector& n) const
{
    SVector result(*this);
    result.Mirror(n);
    return result;
}