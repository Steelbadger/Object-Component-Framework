#include "simdQuaternion.h"
#include "Vector3.h";
#include "Vector4.h"
#include <math.h>
#include "Matrix4x4.h"
#include "simd.h"


namespace SIMD
{
	Quaternion::Quaternion(void)
	{
		data = _mm_set1_ps(0);
	}


	Quaternion::~Quaternion(void)
	{
	}

	Quaternion::Quaternion(float s_, float x_, float y_, float z_)
	{
		data = _mm_set_ps(s_, x_, y_, z_);
	}

	Quaternion::Quaternion(const Vector4 & rhs)
	{
		data = _mm_set_ps(0, rhs.x, rhs.y, rhs.z);
	}

	Quaternion::Quaternion(const Vector3 & rhs)
	{
		data = _mm_set_ps(0, rhs.x, rhs.y, rhs.z);
	}

	Quaternion::Quaternion(float s_, Vector3 rhs)
	{
		data = _mm_set_ps(s_, rhs.x, rhs.y, rhs.z);
	}

	Quaternion::Quaternion(float s_, Vector4 rhs)
	{
		data = _mm_set_ps(s_, rhs.x, rhs.y, rhs.z);
	}

	Quaternion::Quaternion(__m128 d):
		data(d)
	{
	}

	Quaternion::Quaternion(Floats &d):
		data(d.GetData())
	{
	}

	void Quaternion::Rotation(float angle, float xComp, float yComp, float zComp)
	{
		Floats I(0.0f, xComp, yComp, zComp);
		I = I/I.Length();
		Floats smask(1, 0, 0, 0);
		Floats mask(0, 1, 1, 1);
		Floats s = Cosine(Floats(angle/2));
		Floats vs = Sine(Floats(angle/2));
		
		Floats result = (s * smask) + (I * mask);

		data = result.GetData();
	}

	void Quaternion::Rotation(float angle, Vector3 axis)
	{
		Rotation(angle, axis.x, axis.y, axis.z);
	}

	float Quaternion::Modulus() const
	{
		Floats temp(data);
		temp = temp*temp;
		float lengthSqr = temp.Sum();
		return sqrt(lengthSqr);
	}

	float Quaternion::ModulusSqr() const
	{
		Floats temp(data);
		temp = temp*temp;
		float lengthSqr = temp.Sum();
		return (lengthSqr);
	}

	Quaternion Quaternion::Conjugate() const
	{
		Floats conj(data);
		Floats mask(1, -1, -1, -1);

		return Quaternion(conj * mask);
	}

	Quaternion Quaternion::Inverse() const
	{
		if (ModulusSqr() == 1.0) {
			return Conjugate();
		} else {
			return Conjugate()/Modulus();
		}
	}

	void Quaternion::NormalizeSelf()
	{
		if (ModulusSqr() != 1.0) {
			__m128 l = _mm_set1_ps(Modulus());
			data = _mm_div_ps(data, l);
		}
	}

	Quaternion Quaternion::Normalize() const
	{
		if (ModulusSqr() == 1.0) {
			return Quaternion(data);
		} else {
			__m128 l = _mm_set1_ps(Modulus());
			return Quaternion(_mm_div_ps(data, l));
		}
	}

	void Quaternion::NormalizeVector()
	{
		Floats mask(0, 1, 1, 1);
		Floats dat(data);

		Quaternion I(dat*mask);
		I.NormalizeSelf();
		Floats i(I.GetData());
		Floats mask2(1, 0, 0, 0);

		Floats result = (mask2 * dat) + (mask * i);

		data = result.GetData();
	}

	float Quaternion::GetAngle() const
	{
		float s = data.m128_f32[3];
		return acos(s)*2*180/Matrix4x4::PI;
	}

	Vector3 Quaternion::GetAxis() const
	{
		float s = data.m128_f32[3];
		float x = data.m128_f32[2];
		float y = data.m128_f32[1];
		float z = data.m128_f32[0];
		return Vector3(Vector4(x,y,z,1.0f)/asin(s)*2);
	}

	void Quaternion::Align(Vector3 v1, Vector3 v2)
	{
		if (v1 == v2) {
			data = _mm_set_ps(1, 0, 0, 0);
		} else {
			Floats A = Floats(v1.x, v1.y, v1.z, 1.0f);
			Floats B = Floats(v2.x, v2.y, v2.z, 1.0f);
			A /= A.Length();
			float s = acos(Dot(A, B));
			Floats axis = Cross(A, B);

			data = _mm_set_ps(s, axis.x(), axis.y(), axis.z());
		}
	}

	Matrix4x4 Quaternion::GetRotationMatrix()
	{
		float x = X();
		float y = Y();
		float z = Z();
		float s = S();

		float xx = x * x;
		float xy = x * y;
		float xz = x * z;
		float xs = x * s;

		float yy = y * y;
		float yz = y * z;
		float ys = y * s;

		float zz = z * z;
		float zs = z * s;

		float ss = s * s;

		Matrix4x4 out(Matrix4x4::IDENTITY);

		out(0, 0) = ss+xx-yy-zz;
		out(1, 0) = 2*xy-2*zs;
		out(2, 0) = 2*(xz+ys);
		out(0, 1) = 2*(xy+zs);
		out(1, 1) = ss-xx+yy-zz;
		out(2, 1) = 2*(yz-xs);
		out(0, 2) = 2*(xz-ys);
		out(1, 2) = 2*(yz+xs);
		out(2, 2) = ss-xx-yy+zz;
		return out;
	}

	float Quaternion::S() const
	{
		return data.m128_f32[3];
	}

	float Quaternion::X() const
	{
		return data.m128_f32[2];
	}

	float Quaternion::Y() const
	{
		return data.m128_f32[1];
	}

	float Quaternion::Z() const
	{
		return data.m128_f32[0];
	}

	__m128 Quaternion::GetData() const
	{
		return data;
	}


	std::ostream& operator<< (std::ostream& out, const Quaternion& lhs)
	{
		out << "(" << lhs.S() << ", (" << lhs.X() << ", " << lhs.Y() << ", " << lhs.Z() << "))";
		return out;
	}

}