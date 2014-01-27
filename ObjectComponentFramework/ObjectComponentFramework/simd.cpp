#include "simd.h"
#include <smmintrin.h>

namespace SIMD {

/////////////////FLOATING POINT CLASS\\\\\\\\\\\\\\\\\\\\\\\\

	Floats::Floats()
	{
		data = _mm_set1_ps(0);
	}

	Floats::Floats(float x, float y, float z, float w)
	{
		data = _mm_set_ps(x, y, z, w);
	}

	Floats::Floats(float all) 
	{
		data = _mm_set1_ps(all);
	}

	Floats::Floats(float* pointer)
	{
		data = _mm_loadu_ps(pointer);
	}

	Floats::Floats(const Integers& ints)
	{
		__m128i temp = ints.GetData();
		data = _mm_cvtepi32_ps(temp);
	}

	Floats::Floats(__m128 d)
	{
		data = d;
	}

	float Floats::x() const
	{
		return data.m128_f32[3];
	}

	float Floats::y() const
	{
		return data.m128_f32[2];
	}

	float Floats::z() const
	{
		return data.m128_f32[1];
	}

	float Floats::w() const
	{
		return data.m128_f32[0];
	}

	float Floats::Sum() const
	{
		const int mask = 0xF1;
		__m128 tmp = _mm_set1_ps(1.0);
		__m128 output = _mm_dp_ps(data, tmp, mask);
		return output.m128_f32[0];
	}

	float Floats::Length() const
	{
		Floats temp = *this;
		Floats mul(1.0, 1.0, 1.0, 0.0);
		temp *= mul;
		temp = temp*temp;
		float lengthSqr = temp.Sum();
		return sqrt(lengthSqr);
	}


	Floats& Floats::operator += (const Floats& rhs)
	{
		__m128 temp = _mm_add_ps(data, rhs.data);
		data = temp;
		return *this;
	}

	Floats& Floats::operator -= (const Floats& rhs)
	{
		__m128 temp = _mm_sub_ps(data, rhs.data);
		data = temp;
		return *this;
	}

	Floats& Floats::operator *= (const Floats& rhs)
	{
		__m128 temp = _mm_mul_ps(data, rhs.data);
		data = temp;
		return *this;
	}

	Floats& Floats::operator /= (const Floats& rhs)
	{
		__m128 temp = _mm_div_ps(data, rhs.data);
		data = temp;
		return *this;
	}

	float Floats::operator [] (const int index)
	{
		return data.m128_f32[3-index];
	}

	Floats Floats::operator > (const Floats& lhs) const
	{
		__m128 temp = _mm_cmpgt_ps(data, lhs.data);
		return Floats(temp);
	}

	Floats Floats::operator < (const Floats& lhs) const
	{
		__m128 temp = _mm_cmplt_ps(data, lhs.data);
		return Floats(temp);
	}

	std::ostream& operator<< (std::ostream& out, const Floats& lhs)
	{
		out << "(" << lhs.x() << ", " << lhs.y() << ", " << lhs.z() << ", " << lhs.w() << ")";
		return out;
	}

	Floats& Floats::operator = (const Floats& lhs)
	{
		data = lhs.data;
		return *this;
	}

	bool Floats::operator == (const Floats& rhs) const
	{
		__m128 temp = _mm_xor_ps(data, rhs.data);

		for (int i = 0; i < 4; i++) {
			if (temp.m128_f32[i] != 0) {
				return false;
			}
		}
		return true;
	}

	bool Floats::operator != (const Floats& rhs) const
	{
		return !(*this==rhs);
	}

//////////////////////////INTEGERS CLASS\\\\\\\\\\\\\\\\\\\\\\\\\\\\


	Integers::Integers()
	{
		data = _mm_set1_epi32(0);
	}

	Integers::Integers(int x, int y, int z, int w)
	{
		data = _mm_set_epi32(x, y, z, w);
	}

	Integers::Integers(int all)
	{
		data = _mm_set1_epi32(all);
	}

	Integers::Integers(int* pointer)
	{
		data = _mm_set_epi32(pointer[0], pointer[1], pointer[2], pointer[3]);
	}

	Integers::Integers(__m128i d)
	{
		data = d;
	}

	Integers::Integers(const Floats& floats)
	{
		__m128 temp = floats.GetData();
		data = _mm_cvttps_epi32(temp);
	}

	int Integers::x() const
	{
		return data.m128i_i32[0];
	}

	int Integers::y() const
	{
		return data.m128i_i32[1];
	}

	int Integers::z() const
	{
		return data.m128i_i32[2];
	}

	int Integers::w() const
	{
		return data.m128i_i32[3];
	}


	Integers& Integers::operator += (const Integers& rhs)
	{
		__m128i temp = _mm_add_epi32(data, rhs.data);
		data = temp;
		return *this;
	}

	Integers& Integers::operator -= (const Integers& rhs)
	{
		__m128i temp = _mm_sub_epi32(data, rhs.data);
		data = temp;
		return *this;
	}

	Integers& Integers::operator *= (const Integers& rhs)
	{
		__m128i temp = _mm_mullo_epi32(data, rhs.data);
		data = temp;
		return *this;
	}

	Integers Integers::operator > (const Integers& lhs) const
	{
		__m128i temp = _mm_cmpgt_epi32(data, lhs.data);
		return Integers(temp);
	}

	Integers Integers::operator < (const Integers& lhs) const
	{
		__m128i temp = _mm_cmplt_epi32(data, lhs.data);
		return Integers(temp);
	}

	int Integers::operator [] (const int index)
	{
		return data.m128i_i32[3-index];
	}

	bool Integers::operator == (const Integers& rhs) const
	{
		__m128i temp = _mm_xor_si128(data, rhs.data);

		for (int i = 0; i < 4; i++) {
			if (temp.m128i_i32[i] != 0) {
				return false;
			}
		}
		return true;
	}

	bool Integers::operator != (const Integers& rhs) const
	{
		return !(*this==rhs);
	}


//////////////////////FLOATING POINT OPERATORS\\\\\\\\\\\\\\\\\\\\\\\\\\\

	Floats operator + (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_add_ps(lhs.GetData(), rhs.GetData());
		Floats output(temp);
		return output;
	}

	Floats operator - (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_sub_ps(lhs.GetData(), rhs.GetData());
		Floats output(temp);
		return output;
	}

	Floats operator * (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_mul_ps(lhs.GetData(), rhs.GetData());
		Floats output(temp);
		return output;
	}

	Floats operator / (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_div_ps(lhs.GetData(), rhs.GetData());
		Floats output(temp);
		return output;
	}

	Floats operator + (const Floats& lhs, const float& rhs)
	{
		Floats temp(rhs);
		Floats output = lhs + temp;
		return output;
	}

	Floats operator + (const float& lhs, const Floats& rhs)
	{
		Floats temp(lhs);
		Floats output = rhs + temp;
		return output;
	}

	Floats operator - (const Floats& lhs, const float& rhs)
	{
		Floats temp(rhs);
		Floats output = lhs - temp;
		return output;
	}

	Floats operator - (const float& lhs, const Floats& rhs)
	{
		Floats temp(lhs);
		Floats output = temp - rhs;
		return output;
	}

	Floats operator * (const Floats& lhs, const float& rhs)
	{
		Floats temp(rhs);
		Floats output = lhs * temp;
		return output;
	}

	Floats operator * (const float& lhs, const Floats& rhs)
	{
		Floats temp(lhs);
		Floats output = rhs * temp;
		return output;
	}

	Floats operator / (const Floats& lhs, const float& rhs)
	{
		Floats temp(rhs);
		Floats output = lhs/temp;
		return output;
	}

	Floats operator / (const float& lhs, const Floats& rhs)
	{
		Floats temp(lhs);
		Floats output = temp/rhs;
		return output;
	}

	Floats Cross(const Floats& lhs, const Floats& rhs)
	{
		__m128 left = _mm_set_ps(lhs.x(), lhs.y(), lhs.z(), 0);
		__m128 right = _mm_set_ps(rhs.x(), rhs.y(), rhs.z(), 0);

		__m128 temp1 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(2, 1, 3, 0));
		__m128 temp2 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(1, 3, 2, 0));

		__m128 temp3 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(1, 3, 2, 0));
		__m128 temp4 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(2, 1, 3, 0));

		__m128 temp5 = _mm_mul_ps(temp1, temp2);
		__m128 temp6 = _mm_mul_ps(temp3, temp4);
		
		__m128 output = _mm_sub_ps(temp5, temp6);

		Floats homogenise(0,0,0,1);
		Floats out = output;

		return out+homogenise;
	}

	float Dot(const Floats& lhs, const Floats& rhs)
	{
		const int mask = 0xE1;
		__m128 output = _mm_dp_ps(lhs.GetData(), rhs.GetData(), mask);
		return output.m128_f32[0];
	}

	Floats Cosine(const Floats& values)
	{
		Floats x = values + 1.57079632;
		Floats logic = x > 3.14159265;
		Floats sub(6.28318531);
		sub = sub & logic;
		x -= sub;

		logic = x < -3.14159265;
		sub = 6.28318531;
		sub = sub & logic;
		x += sub;
		
		logic = x < 0.0f;

		sub = 2.0f & logic;
		sub -= 1.0f;

		Floats cos = (1.27323954 * x) + (0.405284735 * x * x * sub);


		logic = cos < 0.0f;

		sub = -2.0f & logic;
		sub += 1.0f;

		cos = .225 * (cos * cos * sub - cos) + cos;

		return cos;
	}

	Floats Sine(const Floats& values)
	{
		Floats x = values;
		Floats logic = x > 3.14159265;
		Floats sub(6.28318531);
		sub = sub & logic;
		x -= sub;

		logic = x < -3.14159265;
		sub = 6.28318531;
		sub = sub & logic;
		x += sub;
		
		logic = x < 0.0f;

		sub = 2.0f & logic;
		sub -= 1.0f;

		Floats sin = (1.27323954 * x) + (0.405284735 * x * x * sub);


		logic = sin < 0.0f;

		sub = -2.0f & logic;
		sub += 1.0f;

		sin = .225 * (sin * sin * sub - sin) + sin;

		return sin;
	}

//////////////////////FLOATING POINT BINARY OPERATORS\\\\\\\\\\\\\\\\\\\\\\\\\\\

	Floats operator | (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_or_ps(lhs.GetData(), rhs.GetData());
		return Floats(temp);
	}

	Floats operator ^ (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_xor_ps(lhs.GetData(), rhs.GetData());
		return Floats(temp);
	}

	Floats operator & (const Floats& lhs, const Floats& rhs)
	{
		__m128 temp = _mm_and_ps(lhs.GetData(), rhs.GetData());
		return Floats(temp);
	}


//////////////////////////INTEGER ARITHMETIC\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	Integers operator + (const Integers& lhs, const Integers& rhs)
	{
		Integers temp = lhs;
		temp += rhs;
		return temp;
	}

	Integers operator - (const Integers& lhs, const Integers& rhs)
	{
		Integers temp = lhs;
		temp -= rhs;
		return temp;
	}

	Integers operator * (const Integers& lhs, const Integers& rhs)
	{
		Integers temp = lhs;
		temp *= rhs;
		return temp;
	}

	Integers operator + (const Integers& lhs, const int rhs)
	{
		Integers temp(rhs);
		temp += lhs;
		return temp;
	}

	Integers operator + (const int lhs, const Integers& rhs)
	{
		Integers temp(lhs);
		temp += rhs;
		return temp;
	}

	Integers operator - (const Integers& lhs, const int rhs)
	{
		Integers temp(rhs);
		Integers output = lhs - temp;
		return output;
	}

	Integers operator - (const int lhs, const Integers& rhs)
	{
		Integers temp(lhs);
		Integers output = temp - rhs;
		return output;
	}

	Integers operator * (const Integers& lhs, const int rhs)
	{
		Integers temp(rhs);
		Integers output = lhs * temp;
		return output;
	}

	Integers operator * (const int lhs, const Integers& rhs)
	{
		Integers temp(lhs);
		Integers output = temp * rhs;
		return output;
	}

///////////////INTEGER BINARY OPERATORS\\\\\\\\\\\\\\

	Integers operator | (const Integers& lhs, const Integers& rhs)
	{
		__m128i temp = _mm_or_si128(lhs.GetData(), rhs.GetData());
		return Integers(temp);
	}

	Integers operator ^ (const Integers& lhs, const Integers& rhs)
	{
		__m128i temp = _mm_xor_si128(lhs.GetData(), rhs.GetData());
		return Integers(temp);
	}

	Integers operator & (const Integers& lhs, const Integers& rhs)
	{
		__m128i temp = _mm_and_si128(lhs.GetData(), rhs.GetData());
		return Integers(temp);
	}

	Integers operator << (const Integers& lhs, const int shift)
	{
		__m128i temp = _mm_slli_epi32(lhs.GetData(), shift);
		return Integers(temp);
	}

	Integers operator >> (const Integers& lhs, const int shift)
	{
		__m128i temp = _mm_srai_epi32(lhs.GetData(), shift);
		return Integers(temp);
	}

	Integers operator ~ (const Integers& lhs)
	{
		__m128i mask = _mm_set1_epi32(0xFFFFFFFF);
		__m128i temp = _mm_andnot_si128(lhs.GetData(), mask);
		return Floats(temp);
	}



////////////////COMPOUND ARITHMETIC\\\\\\\\\\\\\\\


	Floats operator + (const Floats& lhs, const Integers& rhs)
	{
		Floats output = lhs + Floats(rhs);
		return output;
	}

	Floats operator + (const Integers& lhs, const Floats& rhs)
	{
		Floats output = Floats(lhs) + rhs;
		return output;
	}

	Floats operator - (const Floats& lhs, const Integers& rhs)
	{
		Floats output = lhs - Floats(rhs);
		return output;		
	}

	Floats operator - (const Integers& lhs, const Floats& rhs)
	{
		Floats output = Floats(lhs) - rhs;
		return output;		
	}

	Floats operator * (const Floats& lhs, const Integers& rhs)
	{
		Floats output = lhs * Floats(rhs);
		return output;	
	}

	Floats operator * (const Integers& lhs, const Floats& rhs)
	{
		Floats output = Floats(lhs) * rhs;
		return output;	
	}

	Floats operator / (const Floats& lhs, const Integers& rhs)
	{
		Floats output = lhs/Floats(rhs);
		return output;	
	}

	Floats operator / (const Integers& lhs, const Floats& rhs)
	{
		Floats output = Floats(lhs)/rhs;
		return output;	
	}

};