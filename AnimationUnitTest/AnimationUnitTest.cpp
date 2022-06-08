#include "pch.h"
#include "CppUnitTest.h"
#include "../Animation/Vector/Vector.h"
#include "../Animation/Vector/Vector.cpp"
#include <xmmintrin.h>
#include <string>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{

			template<> static std::wstring ToString<SVector>(const SVector& v)
			{
				std::wostringstream line;
				line.precision(8);
				line << "x: " << v.GetX() << "y: " << v.GetY() << "z: " << v.GetZ() << "Unused: " << v.GetUnusedAxis();
				return std::wstring(line.str());
			}
		}
	}
}

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
		TEST_METHOD(EqualityInequalityTests)
		{
			{
				const SVector a(1.f, 2.f, 3.f);
				const SVector equal(1.f, 2.f, 3.f);

				const SVector not_equal_01(0.f, 2.f, 3.f);
				const SVector not_equal_02(1.f, 0.f, 3.f);
				const SVector not_equal_03(1.f, 2.f, 0.f);

				Assert::IsTrue(a == equal);
				Assert::IsFalse(a == not_equal_01);
				Assert::IsFalse(a == not_equal_02);
				Assert::IsFalse(a == not_equal_03);
			}

			{
				const SVector a(1.f, 2.f, 3.f);
				const SVector equal(1.f, 2.f, 3.f);
				const SVector not_equal_01(0.f, 2.f, 3.f);
				const SVector not_equal_02(1.f, 0.f, 3.f);
				const SVector not_equal_03(1.f, 2.f, 0.f);

				Assert::IsFalse(a != equal);
				Assert::IsTrue(a != not_equal_01);
				Assert::IsTrue(a != not_equal_02);
				Assert::IsTrue(a != not_equal_03);

			}
		}
		TEST_METHOD(AdditionTests)
		{
			{
				const SVector a(1.f, 2.f, 3.f);
				const SVector b(4.f, 5.f, 6.f);
				const SVector c = a + b;
				SVector a2(a);
				a2 += b;
				const SVector d = a + 10.f;

				SVector answer(5.f, 7.f, 9.f);
				SVector answer2(11.f, 12.f, 13.f);
				Assert::AreEqual(c, answer);
				Assert::AreEqual(a2, answer);
				Assert::AreEqual(d, answer2);
			}
		}
	};
}
