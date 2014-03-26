#include "simdVector4.h"


namespace SIMD
{
	Vector4::Vector4(void)
	{
	}

	Vector4::Vector4(const float x, const float y, const float z, const float w)
		:data(_mm_set_ps(x, y, z, w))
	{
	}

	Vector4::Vector4(const __m128 d)
		:data(d)
	{
	}

	Vector4::~Vector4()
	{
	}

	Vector4 & Vector4::operator +=(const Vector4 &rhs)
	{
		__m128 v3 = _mm_mul_ps(rhs.GetData(), HOMOGENEOUSMASK);
		data = _mm_add_ps(data, v3);
		return *this;
	}

	Vector4 & Vector4::operator -=(const Vector4 &rhs)
	{
		__m128 v3 = _mm_mul_ps(rhs.GetData(), HOMOGENEOUSMASK);
		data = _mm_sub_ps(data, v3);
		return *this;
	}

	Vector4 & Vector4::operator *=(const float s)
	{
		__m128 mulp = _mm_set_ps1(s);
		mulp = _mm_mul_ps(mulp, HOMOGENEOUSMASK);
		data = _mm_mul_ps(data, mulp);
		data = _mm_add_ps(data, ADDHOMOGENEOUS);

		return *this;
	}

	Vector4 & Vector4::operator /=(const float s)
	{
		__m128 mulp = _mm_set_ps1(s);
		mulp = _mm_mul_ps(mulp, HOMOGENEOUSMASK);
		mulp = _mm_add_ps(mulp, ADDHOMOGENEOUS);
		data = _mm_div_ps(data, mulp);

		return *this;
	}

	float Vector4::Length() const
	{
		const int mask = 0xE1;
		__m128 output = _mm_dp_ps(data, data, mask);
		return sqrt(output.m128_f32[0]);
	}

	float Vector4::LengthSqr() const
	{
		const int mask = 0xE1;
		__m128 output = _mm_dp_ps(data, data, mask);
		return output.m128_f32[0];
	}


	bool Vector4::operator ==(const Vector4 & rhs) const
	{
		__m128 temp = _mm_cmpneq_ps(data, rhs.data);	//  if an element is the same then temp at that element is 0xffffffff
		const int mask = 0xF1;
		__m128 output = _mm_dp_ps(temp, temp, mask);
		
		return !output.m128_f32[0];
	}

	bool Vector4::operator !=(const Vector4 & rhs) const
	{
		__m128 temp = _mm_cmpeq_ps(data, rhs.data);	//  if an element is the same then temp at that element is 0xffffffff
		const int mask = 0xF1;
		__m128 output = _mm_dp_ps(temp, temp, mask);
		
		return !output.m128_f32[0];
	}

	Vector4 Vector4::Normalise() const
	{
		return (*this / Length());
	}

	void Vector4::NormaliseSelf()
	{
		*this /= Length();
	}

	std::ostream& operator<< (std::ostream& out, const Vector4& lhs)
	{
		out << "(" << lhs.x() << ", " << lhs.y() << ", " << lhs.z() << ", " << lhs.w() << ")";
		return out;
	}


}