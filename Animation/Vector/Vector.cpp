#include "Vector.h"
#include <pmmintrin.h>
#include <sstream>
#include <iomanip>

const SVector SVector::ZeroVector{ 0.f };
const int SVector::MoveMaskPSTrue{ 0x0F };

SVector::SVector()
{
}

SVector::SVector(const float& value)
    : storage{MakeStorage(value)}
{
}

SVector::SVector(const float& x, const float& y, const float& z)
    : storage{MakeStorage(x, y, z)}
{
}

SVector::SVector(const __m128& value)
    : storage{value}
{
    ResetUnusedAxis();
}

/*            Equality            */
bool SVector::operator==(const SVector& rhs) const
{
    const __m128 result = _mm_cmpeq_ps(storage, rhs.storage);
    const int masked_result = _mm_movemask_ps(result);
    const bool is_equal = masked_result == MoveMaskPSTrue; 
    return is_equal;
}

/*            Inequality            */
bool SVector::operator!=(const SVector& rhs) const
{
    const __m128 result = _mm_cmpeq_ps(storage, rhs.storage);
    const int masked = _mm_movemask_ps(result);
    const bool is_unequal = masked != MoveMaskPSTrue;
    return is_unequal;
}

/*            Addition            */
SVector& SVector::operator+=(const SVector& rhs)
{
    storage = _mm_add_ps(storage, rhs.storage);
    return *this;
}

SVector operator+(const SVector& lhs, const SVector& rhs)
{
    return {_mm_add_ps(lhs.storage, rhs.storage)};
}

SVector& SVector::operator+=(const float& value)
{
    storage = _mm_add_ps(storage, MakeStorage(value));
    return *this;
}

SVector operator+(const SVector& vec, const float& value)
{
    return {_mm_add_ps(vec.storage, SVector::MakeStorage(value))};
}

SVector operator+(const float& value, const SVector& vec)
{
    return {_mm_add_ps(vec.storage, SVector::MakeStorage(value))};
}

/*            Subtraction            */
SVector& SVector::operator-=(const SVector& rhs)
{
    storage = _mm_sub_ps(storage, rhs.storage);
    return *this;
}

SVector operator-(const SVector& lhs, const SVector& rhs)
{
    return {_mm_sub_ps(lhs.storage, rhs.storage)};
}

SVector& SVector::operator-=(const float& value)
{
    storage = _mm_sub_ps(storage, MakeStorage(value));
    return *this;
}

SVector operator-(const SVector& vec, const float& value)
{
    return {_mm_sub_ps(vec.storage, SVector::MakeStorage(value))};
}

SVector operator-(const float& value, const SVector& vec)
{
    return {_mm_sub_ps(SVector::MakeStorage(value), vec.storage)};
}

/*            Multiplication            */
SVector& SVector::operator*=(const SVector& rhs)
{
    storage = _mm_mul_ps(storage, rhs.storage);
    return *this;
}

SVector operator*(const SVector& lhs, const SVector& rhs)
{
    return {_mm_mul_ps(lhs.storage, rhs.storage)};
}

SVector& SVector::operator*=(const float& value)
{
    storage = _mm_mul_ps(storage, MakeStorage(value));
    return *this;
}

SVector operator*(const SVector& vec, const float& value)
{
    return {_mm_mul_ps(vec.storage, SVector::MakeStorage(value))};
}

SVector operator*(const float& value, const SVector& vec)
{
    return {_mm_mul_ps(vec.storage, SVector::MakeStorage(value))};
}

/*            Division            */
SVector& SVector::operator/=(const SVector& rhs)
{
    storage = _mm_div_ps(storage, rhs.storage);
    components[U_INDEX] = 0.f;
    return *this;
}

SVector operator/(const SVector& lhs, const SVector& rhs)
{
    SVector result(_mm_div_ps(lhs.storage, rhs.storage));
    result.components[SVector::U_INDEX] = 0.f;
    return result;
}

SVector& SVector::operator/=(const float& value)
{
    storage = _mm_div_ps(storage, MakeDivisor(value));
    return *this;
}

SVector operator/(const SVector& vec, const float& value)
{
    const SVector result(_mm_div_ps(vec.storage, SVector::MakeDivisor(value)));
    return result;
}

SVector operator/(const float& value, const SVector& vec)
{
    SVector result(_mm_div_ps(SVector::MakeStorage(value), vec.storage));
    result.components[SVector::U_INDEX] = 0.f;
    return result;
}

/*            Magnitude            */
float SVector::Magnitude() const
{
    const __m128 squares = _mm_mul_ps(storage, storage);
    
    __m128 sums = _mm_hadd_ps(squares, squares);      // {0.0f + z, y + x, 0.0f + z, y + x }
    sums = _mm_hadd_ps(sums, sums);                   // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}
    
    // extract the lowest component
    const float result = _mm_cvtss_f32(sums); 
    
    return sqrtf(result);
}

float SVector::MagnitudeXY() const
{
    const __m128 squares = _mm_mul_ps(storage, storage);

    const __m128 sums = _mm_hadd_ps(squares, squares);      // {0.0f + z, y + x, 0.0f + z, y + x }

    // let's move 1th element to 0th place in register
    const __m128 shuffled = _mm_shuffle_ps(sums, sums, _MM_SHUFFLE(1, 0, 2, 3));
    
    const float result = _mm_cvtss_f32(shuffled);

    return sqrtf(result);
}

float SVector::SqrMagnitude() const
{
    const __m128 squares = _mm_mul_ps(storage, storage);
    __m128 sums = _mm_hadd_ps(squares, squares);      // {0.0f + z, y + x, 0.0f + z, y + x }
    sums = _mm_hadd_ps(sums, sums);                 // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}

    // let's extract the lowest component
    const float result = _mm_cvtss_f32(sums);

    return result;
}

/*            Normalization            */
bool SVector::IsZero() const
{
    const __m128 result = _mm_cmpeq_ps(storage, ZeroVector.storage);
    const int masked = _mm_movemask_ps(result);
    return masked == MoveMaskPSTrue;
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
    __m128 value = _mm_mul_ps(storage, rhs.storage);
    value = _mm_hadd_ps(value, value);  // {0.0f + z, y + x, 0.0f + z, y + x }
    value = _mm_hadd_ps(value, value);  // {0.0f + z + y + x, y + x + 0.0f + z, 0.0f + z + y + x, y + x + 0.0f + z}

    // extract the lowest component from register
    const float result = _mm_cvtss_f32(value);
    
    return result;
}

/*            Cross Product            */
SVector& SVector::operator^=(const SVector& rhs)
{
    const __m128 a_yzx = _mm_shuffle_ps(storage, storage, _MM_SHUFFLE(Y_INDEX, Z_INDEX, X_INDEX, U_INDEX));
    const __m128 b_yzx = _mm_shuffle_ps(rhs.storage, rhs.storage, _MM_SHUFFLE(Y_INDEX, Z_INDEX, X_INDEX, U_INDEX));
    const __m128 c = _mm_sub_ps(_mm_mul_ps(storage, b_yzx), _mm_mul_ps(a_yzx, rhs.storage));
    storage = _mm_shuffle_ps(c, c, _MM_SHUFFLE(Y_INDEX, Z_INDEX, X_INDEX, U_INDEX));
    return *this;
}

SVector SVector::operator^(const SVector& rhs) const
{
    SVector result(*this);
    result ^= rhs;
    return result;
}

/*            Operator <<            */
std::ostream& operator<<(std::ostream& os, const SVector& rhs)
{
    os << std::fixed << std::setprecision(2) << "(" << rhs.GetX() << ", " << rhs.GetY() << ", " << rhs.GetZ() << ')';
    return os;
}

std::wostream& operator<<(std::wostream& os, const SVector& rhs)
{
    os << std::fixed << std::setprecision(2) << L"(" << rhs.GetX() << L", " << rhs.GetY() << L", " << rhs.GetZ() << L")";
    return os;
}

/*            Operator >>            */
std::istream& operator>>(std::istream& is, SVector& rhs)
{
    std::string input;
    std::getline(is, input);

    auto start_bracer = input.find('(');
    if (start_bracer == std::string::npos)
    {
        return is;
    }

    auto end_bracer = input.find(')');
    if (end_bracer == std::string::npos)
    {
        return is;
    }

    auto first_comma = input.find(',');
    if (first_comma == std::string::npos)
    {
        return is;
    }

    auto second_comma = input.find(',', first_comma + 1);
    if (second_comma == std::string::npos)
    {
        return is;
    }
    
    float value{ 0 };
    {
        std::istringstream x(input.substr(start_bracer + 1, first_comma - start_bracer - 1));
        x >> value;
        rhs.SetX(value);
    }
    
    {
        std::istringstream y(input.substr(first_comma + 1, second_comma - first_comma - 1));
        y >> value;
        rhs.SetY(value);
    }

    {
        std::istringstream z(input.substr(second_comma + 1, end_bracer - second_comma - 1));
        z >> value;
        rhs.SetZ(value);
    }

    return is;
}

std::wistream& operator>>(std::wistream& is, SVector& rhs)
{
    std::wstring input;
    std::getline(is, input);

    auto start_bracer = input.find(L'(');
    if (start_bracer == std::string::npos)
    {
        return is;
    }

    auto end_bracer = input.find(L')');
    if (end_bracer == std::string::npos)
    {
        return is;
    }

    auto first_comma = input.find(L',');
    if (first_comma == std::string::npos)
    {
        return is;
    }

    auto second_comma = input.find(L',', first_comma + 1);
    if (second_comma == std::string::npos)
    {
        return is;
    }

    float value{ 0 };
    {
        std::wistringstream x(input.substr(start_bracer + 1, first_comma - start_bracer - 1));
        x >> value;
        rhs.SetX(value);
    }

    {
        std::wistringstream y(input.substr(first_comma + 1, second_comma - first_comma - 1));
        y >> value;
        rhs.SetY(value);
    }

    {
        std::wistringstream z(input.substr(second_comma + 1, end_bracer - second_comma - 1));
        z >> value;
        rhs.SetZ(value);
    }

    return is;
}

/*            Reflection            */
void SVector::Mirror(const SVector& n)
{
    const SVector normal = n.NormalSafe();
    
    *this -= ( 2.f * (*this | normal) * normal);
}

SVector SVector::Reflection(const SVector& n) const
{
    SVector result(*this);
    result.Mirror(n);
    return result;
}

/*            Negation            */
SVector SVector::operator-() const
{
    const __m128 sign_mask = _mm_set1_ps(-0.0f);
    const __m128 result = _mm_xor_ps(storage, sign_mask);
    return {result};
}

void SVector::Negate()
{
    const __m128 sign_mask = _mm_set1_ps(-0.0f);
    storage = _mm_xor_ps(storage, sign_mask);
}

/*          Projection          */
void SVector::ProjectOnTo(const SVector& v)
{
    *this = (*this | v) / (v | v) * v;
}

void SVector::ProjectOnToNormal(const SVector& n)
{
    *this = (*this | n) * n;
}

SVector SVector::ProjectionOnTo(const SVector& v) const
{
    SVector result{*this};
    result.ProjectOnTo(v);
    return result;
}

SVector SVector::ProjectionOnToNormal(const SVector& n) const
{
    SVector result{*this};
    result.ProjectOnToNormal(n);
    return result;
}

void SVector::RejectTo(const SVector& v)
{
    *this -= this->ProjectionOnTo(v);
}

SVector SVector::RejectionTo(const SVector& v) const
{
    return *this - this->ProjectionOnTo(v);
}

void SVector::RejectToNormal(const SVector& n)
{
    *this -= this->ProjectionOnToNormal(n);
}

SVector SVector::RejectionToNormal(const SVector& n) const
{
    return *this - this->ProjectionOnToNormal(n);
}
