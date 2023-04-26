#include "Quaternion.h"

#include <immintrin.h>


SQuaternion::SQuaternion(float value)
    : storage{_mm_set_ps1(value)}
{
}

SQuaternion::SQuaternion(float x, float y, float z, float w)
    : storage{_mm_set_ps(x, y, z, w)}
{
}

SQuaternion::SQuaternion(const __m128& value)
    : storage{value}
{
}

SQuaternion::SQuaternion(float roll, float pitch, float yaw)
    : storage{_mm_set_ps1(0.f)}
{
    const __m128 angles = _mm_set_ps(roll, pitch, yaw, 0.f);
    const __m128 halves = _mm_mul_ps(angles, _mm_set_ps1(.5f));

    const union
    {
        __m128 v;
        float angles[4];
    } cosines = {_mm_cos_ps(halves)};

    const union
    {
        __m128 v;
        float angles[4];
    } sines = {_mm_sin_ps(halves)};

    components[X_INDEX] = sines.angles[ROLL_INDEX] * cosines.angles[PITCH_INDEX] * cosines.angles[YAW_INDEX] -
        cosines.angles[ROLL_INDEX] * sines.angles[PITCH_INDEX] * sines.angles[YAW_INDEX];

    components[Y_INDEX] = cosines.angles[ROLL_INDEX] * sines.angles[PITCH_INDEX] * cosines.angles[YAW_INDEX] +
        sines.angles[ROLL_INDEX] * cosines.angles[PITCH_INDEX] * sines.angles[YAW_INDEX];

    components[Z_INDEX] = cosines.angles[ROLL_INDEX] * cosines.angles[PITCH_INDEX] * sines.angles[YAW_INDEX] -
        sines.angles[ROLL_INDEX] * sines.angles[PITCH_INDEX] * cosines.angles[YAW_INDEX];

    components[W_INDEX] = cosines.angles[ROLL_INDEX] * cosines.angles[PITCH_INDEX] * cosines.angles[YAW_INDEX] +
        sines.angles[ROLL_INDEX] * sines.angles[PITCH_INDEX] * sines.angles[YAW_INDEX];
}

