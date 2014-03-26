#pragma once
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

#include <iostream>

namespace SIMD
{
	static const __m128 HOMOGENEOUSMASK = _mm_set_ps(1,1,1,0);
	static const __m128 ZERO			= _mm_setzero_ps();
	static const __m128 ADDHOMOGENEOUS	= _mm_set_ps(0,0,0,1);

	class Vector4
	{
	public:
	// Constructors
		Vector4(void);
		Vector4(const float _x, const float _y, const float _z, const float _w);
		Vector4(const __m128 data);
		~Vector4();

		// Operations with other vectors
		Vector4 & operator+=(const Vector4 & rhs);
		Vector4 & operator-=(const Vector4 & rhs);

		//  Output function
		friend std::ostream& operator<< (std::ostream& out, const Vector4& lhs);

		// Special arithmetic
		Vector4 & operator*=(const float s);
		Vector4 & operator/=(const float s);

		//  Comparison Operators
		bool operator==(const Vector4 & rhs) const;
		bool operator!=(const Vector4 & rhs) const;

		//  Getters
		inline float x() const {return data.m128_f32[3];}
		inline float y() const {return data.m128_f32[2];}
		inline float z() const {return data.m128_f32[1];}
		inline float w() const {return data.m128_f32[0];}

		// Miscellaneous
		float Length() const;
		float LengthSqr() const;
		Vector4 Normalise() const;
		void NormaliseSelf();
		inline __m128 GetData() const {return data;}
	private:
		__m128 data;
	};

	inline Vector4 operator + (const Vector4 &v1,
							   const Vector4 &v2)
	{
		__m128 v3 = _mm_mul_ps(v2.GetData(), HOMOGENEOUSMASK);
		return Vector4(_mm_add_ps(v1.GetData(), v3));
	}

	inline Vector4 operator - (const Vector4 &v1,
							   const Vector4 &v2)
	{
		__m128 v3 = _mm_mul_ps(v2.GetData(), HOMOGENEOUSMASK);
		return Vector4(_mm_sub_ps(v1.GetData(), v3));
	}

	inline Vector4 operator - (const Vector4 &v1)
	{
		return Vector4(_mm_sub_ps(ZERO, v1.GetData()));
	}

	inline Vector4 operator * (const Vector4 &v,
							   const float &s)
	{
		__m128 mulp = _mm_set_ps1(s);
		mulp = _mm_mul_ps(mulp, HOMOGENEOUSMASK);
		__m128 res = _mm_mul_ps(v.GetData(), mulp);

		return Vector4(_mm_add_ps(res, ADDHOMOGENEOUS));
	}

	inline Vector4 operator * (const float & s,
							   const Vector4 &v)
	{
		__m128 mulp = _mm_set_ps1(s);
		mulp = _mm_mul_ps(mulp, HOMOGENEOUSMASK);
		__m128 res = _mm_mul_ps(v.GetData(), mulp);

		return Vector4(_mm_add_ps(res, ADDHOMOGENEOUS));
	}

	inline Vector4 operator / (const Vector4 &v,
							   const float & s)
	{
		__m128 mulp = _mm_set_ps1(s);
		mulp = _mm_mul_ps(mulp, HOMOGENEOUSMASK);
		mulp = _mm_add_ps(mulp, ADDHOMOGENEOUS);
		__m128 res = _mm_div_ps(v.GetData(), mulp);

		return Vector4(res);
	}

	inline Vector4 Cross (const Vector4 &v1,
								 const Vector4 &v2)
	{
		__m128 left = _mm_mul_ps(v1.GetData(), HOMOGENEOUSMASK);
		__m128 right = _mm_mul_ps(v2.GetData(), HOMOGENEOUSMASK);

		__m128 temp1 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(2, 1, 3, 0));
		__m128 temp2 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(1, 3, 2, 0));

		__m128 temp3 = _mm_shuffle_ps(left, left, _MM_SHUFFLE(1, 3, 2, 0));
		__m128 temp4 = _mm_shuffle_ps(right, right, _MM_SHUFFLE(2, 1, 3, 0));

		__m128 temp5 = _mm_mul_ps(temp1, temp2);
		__m128 temp6 = _mm_mul_ps(temp3, temp4);
		
		__m128 output = _mm_sub_ps(temp5, temp6);

		return Vector4(output)+ Vector4(ADDHOMOGENEOUS);
	}

	inline float Dot4(const Vector4 &v1,
							 const Vector4 &v2)
	{
		const int mask = 0xF1;
		__m128 output = _mm_dp_ps(v1.GetData(), v2.GetData(), mask);
		return output.m128_f32[0];
	}

	inline float Dot(const Vector4 &v1,
							 const Vector4 &v2)
	{
		const int mask = 0xE1;
		__m128 output = _mm_dp_ps(v1.GetData(), v2.GetData(), mask);
		return output.m128_f32[0];
	}

	inline Vector4 Normalise (const Vector4 &v)
	{
		return v / v.Length();
	}

	inline Vector4 CalcNormal(const Vector4& pointA, const Vector4& pointB, const Vector4& pointC)
	{
		Vector4 A(pointA.GetData());
		Vector4 B(pointB.GetData());
		Vector4 C(pointC.GetData());

		Vector4 AB = B - A;
		Vector4 AC = C - A;

		Vector4 Normal = Cross(AC, AB);
		Normal.NormaliseSelf();
		return Normal;
	}

};
