#include "pch.h"
#include "CppUnitTest.h"
#include "../Animation/Vector/Vector.h"
#include "../Animation/Vector/Vector.cpp"
#include <xmmintrin.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AnimationUnitTest
{
	TEST_CLASS(SVectorTests)
	{
	public:
		
		TEST_METHOD(ConstructorTests)
		{
			{
				const float zero_value{ 0.f };
				const SVector zero_vector(zero_value);
				Assert::AreEqual(zero_vector.GetX(), zero_value);
				Assert::AreEqual(zero_vector.GetY(), zero_value);
				Assert::AreEqual(zero_vector.GetZ(), zero_value);
				Assert::AreEqual(zero_vector.GetUnusedAxis(), zero_value);
			}

			{
				const float x{ 1 }, y{ 2 }, z{ 3 }, zero{0.f};
				const SVector vector(x, y, z);
				Assert::AreEqual(vector.GetX(), x);
				Assert::AreEqual(vector.GetY(), y);
				Assert::AreEqual(vector.GetZ(), z);
				Assert::AreEqual(vector.GetUnusedAxis(), zero);
			}

			{
				const float x{ 1 }, y{ 2 }, z{ 3 }, w{4}, zero{ 0.f };
				union
				{
					__m128 sse_value;
					float float_values[4];
				} value;
				value.sse_value = _mm_set_ps(x, y, z, w);

				const SVector vector(value.sse_value);
				Assert::AreEqual(vector.GetX(), x);
				Assert::AreEqual(vector.GetY(), y);
				Assert::AreEqual(vector.GetZ(), z);
				Assert::AreEqual(vector.GetUnusedAxis(), zero);
			}
		}


	};
}
