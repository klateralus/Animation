// Animation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Vector/Vector.h"

using namespace std;

void PrintVector(const SVector& a)
{
    cout << "X: " << a.GetX() << " Y: " << a.GetY() << " Z: " << a.GetZ() << " Unused: " << a.GetUnusedAxis() << endl;
}

int main()
{
    {
        SVector a(3.0f);
        PrintVector(a);

        SVector b(-1.0f, -2.0f, -3.0f);
        PrintVector(b);

        SVector c{ b };
        PrintVector(c);

        SVector d = a;
        PrintVector(d);
    }

    // zero vector
    {
        PrintVector(SVector::ZeroVector);
    }

    // division by zero
    {
        SVector a (1.f);
        SVector b (1.f);
        SVector c = a / b;
        PrintVector(c);
    }

    // magnitude
    {
        SVector a(1.f, 1.f, 1.f);
        cout << "Magnitude: ";
        PrintVector(a);
    }
    

}
