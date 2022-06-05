// Animation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Vector/Vector.h"

using namespace std;

int main()
{   
    SVector a(3.0f);
    cout << a.GetX() << ' ' << a.GetY() << ' ' << a.GetZ() << endl;

    SVector b(-1.0f, -2.0f, -3.0f);
    cout << b.GetX() << ' ' << b.GetY() << ' ' << b.GetZ() << endl;

    SVector c{ b };
    cout << c.GetX() << ' ' << c.GetY() << ' ' << c.GetZ() << endl;

    SVector d = a;
    cout << d.GetX() << ' ' << d.GetY() << ' ' << d.GetZ() << endl;

    cout << SVector::ZeroVector.GetX() << ' ' << SVector::ZeroVector.GetY() << ' ' << SVector::ZeroVector.GetZ() << endl;

    
}
