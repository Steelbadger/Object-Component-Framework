#pragma once
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

#include "simdVector4.h"

namespace SIMD
{
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const float* data);
		Matrix4x4(	const float f11, const float f12, const float f13, const float f14,
					const float f21, const float f22, const float f23, const float f24,
					const float f31, const float f32, const float f33, const float f34, 
					const float f41, const float f42, const float f43, const float f44	);
		Matrix4x4(const __m128* data);
		Matrix4x4(const __m128& row0, const __m128& row1, const __m128& row2, const __m128& row3);

		~Matrix4x4();

//		void Translation(const Vector4 &vec);
//		void RotationX(float fAngle);
//		void RotationY(float fAngle);
//		void RotationZ(float fAngle);

//		void RotationAroundVectorThroughZero(const Vector4 &Vector, float angle);
//		void Projection(float fov, float aspect, float near, float far);
//		void Transpose();

//		void Scale(const Vector4 &scale);

//		void LookAt(const Vector4 & vFrom,
//					const Vector4 & vTo,
//					const Vector4 & vUp);

		inline float const &operator()( unsigned int Row, unsigned int Column ) const
		{ return rows[Row].m128_f32[3-Column]; }

		// Output
//		friend std::ostream& operator<<(std::ostream& out, Matrix4x4& object);

		static const Matrix4x4 IDENTITY;
		static const Matrix4x4 NULLMATRIX;
		static const double PI;

		inline __m128 GetRow(int i) const {
			return rows[i];
		};

	private:
		__m128 rows[4];
	};

	inline Matrix4x4 Transpose( Matrix4x4 const & M )
	{
		__m128 data[4];
		data[0] = M.GetRow(0);
		data[1] = M.GetRow(1);
		data[2] = M.GetRow(2);
		data[3] = M.GetRow(3);

		_MM_TRANSPOSE4_PS(data[3], data[2], data[1], data[0]);

		return Matrix4x4(data);
	}

	inline Matrix4x4 CollapseMulp (Matrix4x4 const &M1, Matrix4x4 const& M2)
	{
		__m128 out[4];

		const int mask1 = 0xF1;
		const int mask2 = 0xF2;
		const int mask3 = 0xF4;
		const int mask4 = 0xF8;

		__m128 m1[4];
		__m128 m2[4];
		for (int i = 0 ; i < 4; i++) {
			m1[i] = M1.GetRow(i);
			m2[i] = M2.GetRow(i);
			out[i] = ZERO;
		}

		for (int j = 0; j < 4; j++) {
			__m128 res = _mm_dp_ps(m1[j], m2[0], mask4);
			out[j] = _mm_add_ps(out[j], res);

			res = _mm_dp_ps(m1[j], m2[1], mask3);
			out[j] = _mm_add_ps(out[j], res);

			res = _mm_dp_ps(m1[j], m2[2], mask2);
			out[j] = _mm_add_ps(out[j], res);

			res = _mm_dp_ps(m1[j], m2[3], mask1);
			out[j] = _mm_add_ps(out[j], res);
		}

		return Matrix4x4(out);
	}

	inline Matrix4x4 operator * ( const Matrix4x4 & M1,
									const Matrix4x4 & M2 )
	{
		Matrix4x4 temp = Transpose(M2);
		return CollapseMulp(M1, temp);
	}

	inline Vector4 operator * ( const Matrix4x4 & M,
						 const Vector4 & V )
	{
		__m128 result = ZERO;
		Matrix4x4 M2;
		M2 = Transpose(M);
		const int mask1 = 0xF1;
		const int mask2 = 0xF2;
		const int mask3 = 0xF4;
		const int mask4 = 0xF8;

		__m128 temp = _mm_dp_ps(M2.GetRow(3), V.GetData(), mask1);
		result = _mm_add_ps(result, temp);

		temp = _mm_dp_ps(M2.GetRow(2), V.GetData(), mask2);
		result = _mm_add_ps(result, temp);

		temp = _mm_dp_ps(M2.GetRow(1), V.GetData(), mask3);
		result = _mm_add_ps(result, temp);

		temp = _mm_dp_ps(M2.GetRow(0), V.GetData(), mask4);
		result = _mm_add_ps(result, temp);

		return Vector4(result);
	}

	inline Vector4 operator * ( const Vector4 & V,
						 const Matrix4x4 & M )
	{
		__m128 result = ZERO;
		Matrix4x4 M2;
		M2 = Transpose(M);
		const int mask1 = 0xF1;
		const int mask2 = 0xF2;
		const int mask3 = 0xF4;
		const int mask4 = 0xF8;

		__m128 temp = _mm_dp_ps(M2.GetRow(3), V.GetData(), mask1);
		result = _mm_add_ps(result, temp);

		temp = _mm_dp_ps(M2.GetRow(2), V.GetData(), mask2);
		result = _mm_add_ps(result, temp);

		temp = _mm_dp_ps(M2.GetRow(1), V.GetData(), mask3);
		result = _mm_add_ps(result, temp);

		temp = _mm_dp_ps(M2.GetRow(0), V.GetData(), mask4);
		result = _mm_add_ps(result, temp);

		return Vector4(result);
	}

	inline Matrix4x4 operator - ( const Matrix4x4 & M )
	{
		__m128 out[4];
		for (int i = 0; i < 4; i++) {
			out[i] = _mm_sub_ps(ZERO, M.GetRow(i));	
		}
		return Matrix4x4(out);
	}

	inline Matrix4x4 operator - ( const Matrix4x4 & M1,
								  const Matrix4x4 & M2 )
	{
		__m128 out[4];
		for (int i = 0; i < 4; i++) {
			out[i] = _mm_sub_ps(M1.GetRow(i), M2.GetRow(i));	
		}
		return Matrix4x4(out);
	}

	inline Matrix4x4 operator + ( const Matrix4x4 & M1,
								  const Matrix4x4 & M2 )
	{
		__m128 out[4];
		for (int i = 0; i < 4; i++) {
			out[i] = _mm_add_ps(M1.GetRow(i), M2.GetRow(i));	
		}
		return Matrix4x4(out);
	}

	inline Matrix4x4 operator * ( const Matrix4x4 & M,
								  const float & s )
	{
		__m128 out[4];
		__m128 num = _mm_set_ps1(s);
		for (int i = 0; i < 4; i++) {
			out[i] = _mm_mul_ps(M.GetRow(i), num);	
		}
		return Matrix4x4(out);
	}

	inline Matrix4x4 operator * ( const float & s,
								  const Matrix4x4 & M )
	{
		__m128 out[4];
		__m128 num = _mm_set_ps1(s);
		for (int i = 0; i < 4; i++) {
			out[i] = _mm_mul_ps(M.GetRow(i), num);	
		}
		return Matrix4x4(out);
	}

	Matrix4x4 Perspective(const float fov, const float aspect, const float znear, const float nfar);
	Matrix4x4 Orthographic(const float width, const float height, const float near, const float far);
	Matrix4x4 Inverse(const Matrix4x4& in);
};
