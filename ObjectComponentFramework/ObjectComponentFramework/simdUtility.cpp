#include "simdUtility.h"
#include "simd.h"

namespace SIMD{
	Matrix4x4& Multiply(const Matrix4x4 &a, const Matrix4x4 &b)
	{
		const __m128 A[4] = {	_mm_set_ps(a(0, 0), a(0, 1), a(0, 2), a(0, 3)),
								_mm_set_ps(a(1, 0), a(1, 1), a(1, 2), a(1, 3)),
								_mm_set_ps(a(2, 0), a(2, 1), a(2, 2), a(2, 3)),
								_mm_set_ps(a(3, 0), a(3, 1), a(3, 2), a(3, 3))};

		const __m128 B[4] =  {	_mm_set_ps(b(0, 0), b(0, 1), b(0, 2), b(0, 3)),
								_mm_set_ps(b(1, 0), b(1, 1), b(1, 2), b(1, 3)),
								_mm_set_ps(b(2, 0), b(2, 1), b(2, 2), b(2, 3)),
								_mm_set_ps(b(3, 0), b(3, 1), b(3, 2), b(3, 3))};;

		//  Masks
		const int col0 = 0xF1;
		const int col1 = 0xF2;
		const int col2 = 0xF4;
		const int col3 = 0xF8;
		
		__m128 result[4];

		for (int i = 0; i < 4 ; i++) {
			result[i] = _mm_dp_ps(A[i], B[0], col0) + _mm_dp_ps(A[i], B[1], col1) + _mm_dp_ps(A[i], B[2], col2) + _mm_dp_ps(A[i], B[3], col3);
		}

		return Matrix4x4((float*)&result);
	}

	Matrix4x4 ChainMultiply(std::initializer_list<Matrix4x4> mats)
	{
		return Matrix4x4();
	}
}