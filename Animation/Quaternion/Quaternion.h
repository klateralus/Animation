#pragma once
#include <xmmintrin.h>


struct SQuaternion
{
private:
    
    union
    {
        float components[4];
        __m128 storage{ _mm_setzero_ps() };
    };
    
    // index of X component
    constexpr static size_t X_INDEX{ 3 };
    
    // index of Y component
    constexpr static size_t Y_INDEX{ 2 };
    
    // index of Z component
    constexpr static size_t Z_INDEX{ 1 };
    
    // index of W component
    constexpr static size_t W_INDEX{ 0 };

    // index of Roll component
    constexpr static size_t ROLL_INDEX{ 3 };
    
    // index of Pitch component
    constexpr static size_t PITCH_INDEX{ 2 };
    
    // index of Yaw component
    constexpr static size_t YAW_INDEX{ 1 };

public:
    
    explicit SQuaternion(float value);
    SQuaternion(float x, float y, float z, float w);
    explicit SQuaternion(const __m128& value);
    SQuaternion(float roll, float pitch, float yaw);
};
