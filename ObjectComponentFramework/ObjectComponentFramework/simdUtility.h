#pragma once

#include "Matrix4x4.h"
#include <initializer_list>
#include <smmintrin.h>

namespace SIMD{
	Matrix4x4& Multiply(const Matrix4x4 &a, const Matrix4x4 &b);
	Matrix4x4 ChainMultiply(std::initializer_list<Matrix4x4> list);



	inline __m128 operator + (const __m128& lhs, const __m128& rhs)
	{
		return _mm_add_ps(lhs, rhs);
	}

	inline __m128 operator - (const __m128& lhs, const __m128& rhs)
	{
		return _mm_sub_ps(lhs, rhs);
	}

	inline __m128 operator * (const __m128& lhs, const __m128& rhs)
	{
		return _mm_mul_ps(lhs, rhs);
	}

	inline __m128 operator / (const __m128& lhs, const __m128& rhs)
	{
		return _mm_div_ps(lhs, rhs);
	}

	//__m128 Dot(const __m128& lhs, const __m128& rhs, const int mask)
	//{
	//	return _mm_dp_ps(lhs, rhs, mask);
	//}
}