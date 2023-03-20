#include "pch.h"
#include <sstream>
#include <string>
#include <xmmintrin.h>
#include "CppUnitTest.h"
#include "../Animation/Vector/Vector.cpp"
#include "../Animation/Vector/Vector.h"

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
				constexpr float zero_value{ 0.f };
				const SVector zero_vector(zero_value);
				Assert::AreEqual(zero_vector.GetX(), zero_value);
				Assert::AreEqual(zero_vector.GetY(), zero_value);
				Assert::AreEqual(zero_vector.GetZ(), zero_value);
				Assert::AreEqual(zero_vector.GetUnusedAxis(), zero_value);
			}

			{
				constexpr float x{ 1 }, y{ 2 }, z{ 3 }, zero{ 0.f };
				const SVector vector(x, y, z);
				Assert::AreEqual(vector.GetX(), x);
				Assert::AreEqual(vector.GetY(), y);
				Assert::AreEqual(vector.GetZ(), z);
				Assert::AreEqual(vector.GetUnusedAxis(), zero);
			}

			{
				constexpr float x{ 1 }, y{ 2 }, z{ 3 }, w{ 4 }, zero{ 0.f };
				union
				{
					__m128 sse_value;
					float float_values[4];
				} value{};
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
		TEST_METHOD(IsZeroTests)
		{
			const SVector a(1.f);
			const SVector b(0.f);

			Assert::IsFalse(a.IsZero());
			Assert::IsTrue(b.IsZero());
		}
		TEST_METHOD(AdditionTests)
		{
			{
				const SVector a(1.f, 2.f, 3.f);
				const SVector b(4.f, 5.f, 6.f);
				const SVector c = a + b;
				SVector a2(a);
				a2 += b;
				SVector a3(a);
				a3 += 10.f;

				const SVector d = a + 10.f;
				const SVector e = 10.f + a;

				const SVector answer(5.f, 7.f, 9.f);
				const SVector answer2(11.f, 12.f, 13.f);
				Assert::AreEqual(c, answer);
				Assert::AreEqual(a2, answer);
				Assert::AreEqual(a3, answer2);
				Assert::AreEqual(d, answer2);
				Assert::AreEqual(e, answer2);
			}
		}
		TEST_METHOD(SubstractionTests)
		{
			{
				const SVector a(1.f, 2.f, 3.f);
				const SVector b(4.f, 5.f, 6.f);
				const SVector c = b - a;
				SVector b2(b);
				b2 -= a;
				SVector a2(a);
				a2 -= 1.0;

				const SVector d = a - 1.0;
				const SVector e = 10 - a;

				const SVector answer(3.f);
				const SVector answer_02(0.f, 1.f, 2.f);
				const SVector answer_03(9.f, 8.f, 7.f);

				Assert::AreEqual(c, answer);
				Assert::AreEqual(b2, answer);
				Assert::AreEqual(a2, answer_02);
				Assert::AreEqual(d, answer_02);
				Assert::AreEqual(e, answer_03);
			}
		}
		TEST_METHOD(MultiplicationTests)
		{
			{
				const SVector a(1.f);
				const SVector b(3.f);
				const SVector c = a * b;
				const SVector d = a * 3.f;
				const SVector e = 3.f * a;
				SVector a2(a);
				a2 *= b;
				SVector a3(a);
				a3 *= 3.f;

				const SVector answer(3.f);

				Assert::AreEqual(c, answer);
				Assert::AreEqual(d, answer);
				Assert::AreEqual(e, answer);
				Assert::AreEqual(a2, answer);
				Assert::AreEqual(a3, answer);
			}
		}
		TEST_METHOD(DivisionTests)
		{
			{
				const SVector a(3.f);
				const SVector b(1.f);
				const SVector c = a / b;
				const SVector d = a / 1.f;
				const SVector e = 3.f / b;
				SVector a2(a);
				a2 /= b;
				SVector a3(a);
				a3 /= 1.f;

				const SVector answer(3.f);

				Assert::AreEqual(c, answer);
				Assert::AreEqual(d, answer);
				Assert::AreEqual(e, answer);
				Assert::AreEqual(a2, answer);
				Assert::AreEqual(a3, answer);
			}
		}
		TEST_METHOD(MagnitudeTests)
		{
			{
				const SVector a(1.f, 0.f, 0.f);
				const float magnitude = a.Magnitude();
				const float length = a.Length();
				constexpr float answer = 1.f;
				const SVector b(4.f, 3.f, 0.f);
				const float magnitude_02 = b.Magnitude();
				constexpr float answer_02 = 5.f;


				Assert::AreEqual(magnitude, answer);
				Assert::AreEqual(length, answer);
				Assert::AreEqual(magnitude_02, answer_02);
			}

			{
				const SVector a(4.f, 0.f, 0.f);
				const float square_magnitude = a.SqrMagnitude();
				const float square_length = a.SqrLength();
				constexpr float answer = 16.f;

				Assert::AreEqual(square_magnitude, answer);
				Assert::AreEqual(square_length, answer);
			}

			{
				const SVector a(4.f, 3.f, 9.f);
				const float mag_xy = a.MagnitudeXY();
				const float len_xy = a.LengthXY();
				constexpr float answer = 5.f;

				Assert::AreEqual(mag_xy, answer);
				Assert::AreEqual(len_xy, answer);
			}
		}
		TEST_METHOD(NormalizationTests)
		{
			{
				SVector a(15.f, 0.f, -13.f);
				a.Normalize();
				const float length = a.Length();

				Assert::AreEqual(length, 1.f);
			}

			{
				SVector b(SVector::ZeroVector);
				b.NormalizeSafe();
				const float len = b.Length();

				Assert::AreNotEqual(len, 1.f);
			}

			{
				const SVector a(4.f, 3.f, 0.f);
				const SVector b(a.Normal());
				const float len_a = a.Length();
				const float len_b = b.Length();

				Assert::AreEqual(len_a, 5.f);
				Assert::AreEqual(len_b, 1.f);
			}

			{
				const SVector a(SVector::ZeroVector);
				const SVector b(a.NormalSafe());
				const float len_a = a.Length();
				const float len_b = b.Length();

				Assert::AreEqual(len_a, 0.f);
				Assert::AreNotEqual(len_b, 1.f);
			}

			{
				const SVector a(4.f, 3.f, 0.f);
				const SVector b(a.Unit());
				const float len_a = a.Length();
				const float len_b = b.Length();

				Assert::AreEqual(len_a, 5.f);
				Assert::AreEqual(len_b, 1.f);
			}

			{
				const SVector a(SVector::ZeroVector);
				const SVector b(a.UnitSafe());
				const float len_a = a.Length();
				const float len_b = b.Length();

				Assert::AreEqual(len_a, 0.f);
				Assert::AreNotEqual(len_b, 1.f);
			}
		}
		TEST_METHOD(DotProductTests)
		{
			{
				const SVector x(1.0f, 0.f, 0.f);
				const SVector y(0.0f, 1.f, 0.f);
				const SVector x_n(-1.0f, 0.f, 0.f);
			
				const float dir = x | x;
				const float dir_per = x | y;
				const float dir_op = x | x_n;


				Assert::AreEqual(dir, 1.0f);
				Assert::AreEqual(dir_per, 0.0f);
				Assert::AreEqual(dir_op, -1.0f);
			}
			{
				const SVector x(1.0f, 0.f, 0.f);
				const SVector y(0.0f, 1.f, 0.f);
				const SVector x_n(-1.0f, 0.f, 0.f);

				const float dir = SVector::DotProduct(x, x);
				const float dir_per = SVector::DotProduct(x, y);
				const float dir_op = SVector::DotProduct(x,x_n);


				Assert::AreEqual(dir, 1.0f);
				Assert::AreEqual(dir_per, 0.0f);
				Assert::AreEqual(dir_op, -1.0f);
			}
			{
				const SVector x(1.0f, 0.f, 0.f);
				const SVector y(0.0f, 1.f, 0.f);
				const SVector x_n(-1.0f, 0.f, 0.f);

				const float dir = x.DotProduct(x);
				const float dir_per = x.DotProduct(y);
				const float dir_op = x.DotProduct(x_n);


				Assert::AreEqual(dir, 1.0f);
				Assert::AreEqual(dir_per, 0.0f);
				Assert::AreEqual(dir_op, -1.0f);
			}
		}
		TEST_METHOD(CrossProductTests)
		{
			{
				const SVector x(1.0f, 0.f, 0.f);
				const SVector y(0.0f, 1.f, 0.f);
				const SVector z(0.0f, 0.f, 1.f);
				const SVector r = x ^ y;

				Assert::AreEqual(r, z);
			}
			{
				SVector x(1.0f, 0.f, 0.f);
				const SVector y(0.0f, 1.f, 0.f);
				const SVector z(0.0f, 0.f, 1.f);
				x ^= y;

				Assert::AreEqual(x, z);
			}
		}
		TEST_METHOD(StreamOperatorsTests)
		{
			{
				const SVector x(1.5f, 1.3f, 1.0f);
				std::ostringstream output;
				output << x;
				const std::string x_s(output.str());
				const std::string result("(1.50, 1.30, 1.00)");

				Assert::AreEqual(x_s, result);
			}

			{
				const SVector x(1.5f, 1.3f, 1.0f);
				std::wostringstream output;
				output << x;
				const std::wstring x_s(output.str());
				const std::wstring result(L"(1.50, 1.30, 1.00)");

				Assert::AreEqual(x_s, result);
			}

			{
				const SVector x(1.f, 3.f, 4.f);
				std::ostringstream output;
				output << x;
				std::istringstream input(output.str());
				SVector a;
				input >> a;

				Assert::AreEqual(x, a);
			}

			{
				const SVector x(1.f, 3.f, 4.f);
				std::wostringstream output;
				output << x;
				std::wistringstream input(output.str());
				SVector a;
				input >> a;

				Assert::AreEqual(x, a);
			}
		}
	};
}
