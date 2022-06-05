#pragma once

template<typename T>
struct SJoint
{
    T Translation[3];
    T Quaternion[4];
    T Scale[3];
};

