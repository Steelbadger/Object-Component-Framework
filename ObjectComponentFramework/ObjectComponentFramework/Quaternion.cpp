#include "Quaternion.h"
#include "Vector3.h";
#include "Vector4.h"
#include <math.h>
#include "Matrix4x4.h"


Quaternion::Quaternion(void):
s(1), x(0), y(0), z(0)
{
}


Quaternion::~Quaternion(void)
{
}

Quaternion::Quaternion(float s_, float x_, float y_, float z_):
	s(s_), x(x_), y(y_), z(z_)
{
}

Quaternion::Quaternion(const Vector4 & rhs):
	s(0), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Quaternion::Quaternion(const Vector3 & rhs):
	s(0), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Quaternion::Quaternion(float s_, Vector3 rhs):
	s(s_), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

Quaternion::Quaternion(float s_, Vector4 rhs):
	s(s_), x(rhs.x), y(rhs.y), z(rhs.z)
{
}

void Quaternion::Rotation(float angle, float xComp, float yComp, float zComp)
{
	Quaternion I(0.0f, xComp, yComp, zComp);
	I.NormalizeSelf();
	s = cos(angle/2);
	float vs = sin(angle/2);
	x = I.x*vs;
	y = I.y*vs;
	z = I.z*vs;
}

void Quaternion::Rotation(float angle, Vector3 axis)
{
	Rotation(angle, axis.x, axis.y, axis.z);
}

float Quaternion::Modulus() const
{
	return sqrt(s*s+x*x+y*y+z*z);
}

float Quaternion::ModulusSqr() const
{
	return (s*s+x*x+y*y+z*z);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(s, -x, -y, -z);
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
		float l = Modulus();
		s /= l;
		x /= l;
		y /= l;
		z /= l;
	}
}

Quaternion Quaternion::Normalize() const
{
	if (ModulusSqr() == 1.0) {
		return Quaternion(s, x, y, z);
	} else {
		float l = Modulus();
		return Quaternion(s/l, x/l, y/l, z/l);
	}
}

void Quaternion::NormalizeVector()
{
	Quaternion I(0.0f, x, y, z);
	I.NormalizeSelf();
	x = I.x;
	y = I.y;
	z = I.z;
}

float Quaternion::GetAngle() const
{
	return acos(s)*2*180/Matrix4x4::PI;
}

Vector3 Quaternion::GetAxis() const
{
	return Vector3(Vector4(x,y,z,1.0f)/asin(s)*2);
}

void Quaternion::Align(Vector3 v1, Vector3 v2)
{
	if (v1 == v2) {
		s = 1.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	} else {
		Vector4 A = Vector4(v1).Normalise();
		Vector4 B = Vector4(v2).Normalise();
		Quaternion res(acos(Dot3(A, B)), Cross(A, B));
		s = res.s;
		x = res.x;
		y = res.y;
		z = res.z;
	}
}

Matrix4x4 Quaternion::GetRotationMatrix()
{

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
	//out.elem[0][0] = ss+xx-yy-zz;
	//out.elem[1][0] = 2*xy-2*zs;
	//out.elem[2][0] = 2*(xz+ys);
	//out.elem[0][1] = 2*(xy+zs);
	//out.elem[1][1] = ss-xx+yy-zz;
	//out.elem[2][1] = 2*(yz-xs);
	//out.elem[0][2] = 2*(xz-ys);
	//out.elem[1][2] = 2*(yz+xs);
	//out.elem[2][2] = ss-xx-yy+zz;

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


std::ostream& operator<< (std::ostream& out, const Quaternion& lhs)
{
	out << "(" << lhs.s << ", (" << lhs.x << ", " << lhs.y << ", " << lhs.z << "))";
	return out;
}