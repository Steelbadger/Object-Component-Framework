#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <iostream>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "simd.h"

namespace SIMD
{
	class Quaternion
	{
	public:
		Quaternion(void);
		Quaternion(float s, float x, float y, float z);
		Quaternion(const Vector4 & rhs);
		Quaternion(const Vector3 & rhs);
		Quaternion(float s, Vector3 rhs);
		Quaternion(float s, Vector4 rhs);
		Quaternion(__m128 data);
		Quaternion(Floats &data);
		~Quaternion(void);

		void Rotation(float angle, float xComp, float yComp, float zComp);
		void Rotation(float angle, Vector3 axis);
		void Align(Vector3 v1, Vector3 v2);

		friend std::ostream& operator<< (std::ostream& out, const Quaternion& lhs);

		float Modulus() const;
		float ModulusSqr() const;
		float S() const;
		float X() const;
		float Y() const;
		float Z() const;

		Quaternion Conjugate() const;
		Quaternion Inverse() const;
		Quaternion Normalize() const;
		void NormalizeSelf();
		void NormalizeVector();
		float GetAngle() const;
		Vector3 GetAxis() const;
		__m128 GetData() const;
		Matrix4x4 GetRotationMatrix();
	private:
		__m128 data;
	};

	inline Quaternion operator + (const Quaternion &q1,
							   const Quaternion &q2)
	{
		return Quaternion(_mm_add_ps(q1.GetData(), q2.GetData()));
	}

	inline Quaternion operator - (const Quaternion &q1,
							   const Quaternion &q2)
	{
		return Quaternion(_mm_sub_ps(q1.GetData(), q2.GetData()));
	}

	inline Quaternion operator * (const Quaternion &q1,
							   const Quaternion &q2)
	{
		__m128 s, x, y, z;
		__m128 qu1 = q1.GetData();
		__m128 sign = _mm_set_ps(1, -1, -1, -1);
		__m128 qu2 = q2.GetData();
		__m128 q2s = _mm_mul_ps(qu2, sign);
		__m128 q2x = _mm_shuffle_ps(qu2, qu2, _MM_SHUFFLE(2, 3, 0, 1));
		sign = _mm_set_ps(1, 1, 1, -1);
		q2x = _mm_mul_ps(q2x, sign);
		__m128 q2y = _mm_shuffle_ps(qu2, qu2, _MM_SHUFFLE(1, 0, 3, 2));
		sign = _mm_set_ps(1, -1, 1, 1);
		q2y = _mm_mul_ps(q2y, sign);
		__m128 q2z = _mm_shuffle_ps(qu2, qu2, _MM_SHUFFLE(0, 2, 1, 3));
		sign = _mm_set_ps(1, 1, -1, 1);

		s = _mm_mul_ps(qu1, q2s);
		x = _mm_mul_ps(qu1, q2x);
		y = _mm_mul_ps(qu1, q2y);
		z = _mm_mul_ps(qu1, q2z);
		
		const int smask = 0xF1;
		const int xmask = 0xF2;
		const int ymask = 0xF4;
		const int zmask = 0xF8;
		__m128 tmp = _mm_set1_ps(1.0);
		s = _mm_dp_ps(s, tmp, smask);
		x = _mm_dp_ps(x, tmp, xmask);
		y = _mm_dp_ps(y, tmp, ymask);
		z = _mm_dp_ps(z, tmp, zmask);

		__m128 final = _mm_add_ps(_mm_add_ps(s, x), _mm_add_ps(y, z));

		return Quaternion(final);
	}


	inline Quaternion operator * (const Quaternion &q,
							   const float &c)
	{
		__m128 multiplier = _mm_set1_ps(c);
		return Quaternion(_mm_mul_ps(q.GetData(), multiplier));
	}

	inline Quaternion operator * (const float &c,
							   const Quaternion &q)
	{
		__m128 multiplier = _mm_set1_ps(c);
		return Quaternion(_mm_mul_ps(q.GetData(), multiplier));
	}

	inline Quaternion operator / (const Quaternion &q,
							   const float & c)
	{
		__m128 multiplier = _mm_set1_ps(c);
		return Quaternion(_mm_div_ps(q.GetData(), multiplier));
	}

	inline Quaternion Lerp(const Quaternion &q1, const Quaternion &q2, const float t)
	{
		__m128 T = _mm_set1_ps(t);
		__m128 invT = _mm_set1_ps(1-t);
		__m128 qu1 = q1.GetData();
		__m128 qu2 = q2.GetData();
		
		return Quaternion(_mm_add_ps(_mm_mul_ps(invT, qu1), _mm_mul_ps(T, qu2)));
	}

	inline float Dot(const Quaternion &q1, const Quaternion &q2)
	{
		const int mask = 0xF1;
		__m128 num = _mm_dp_ps(q1.GetData(), q2.GetData(), mask);
		return (num.m128_f32[0]);
	}

	inline Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, const float t)
	{
		float angle = Dot(q2, q1);
		// make sure we use the short rotation
		Quaternion c1 = q1;

		if (angle < 0.0f) {
			c1 = q1 * -1.0f;
			angle *= -1.0f;
		}

		if (angle <= (1-0.05)) {
			const float theta = acosf(angle);
			const float invSinTheta = 1/(sinf(theta));
			const float scale = sinf(theta * (1.0f-t)) * invSinTheta;
			const float invscale = sinf(theta * t) * invSinTheta;
			return ((c1*scale) + (q2*invscale));
		} else {
			return Lerp(c1,q2,t);
		}
	}


}