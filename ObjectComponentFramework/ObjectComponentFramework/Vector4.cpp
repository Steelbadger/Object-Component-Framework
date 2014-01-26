#include "Vector4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <stdio.h>

Vector4::Vector4(void)
{
}

Vector4::Vector4(const float _x, const float _y, const float _z, const float _w)
	:x(_x),y(_y),z(_z),w(_w)
{
}

Vector4::Vector4(const Vector4 &rhs)
	:x(rhs.x),y(rhs.y),z(rhs.z),w(rhs.w)
{
}

Vector4::Vector4(const Vector3 &rhs):
	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
{
}

Vector4::Vector4(const Quaternion &rhs):
	x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
{
}

Vector4::~Vector4(void)
{
}

Vector4 & Vector4::operator +=(const Vector4 &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	w = 1.0f;

	return *this;
}

Vector4 & Vector4::operator -=(const Vector4 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	w = 1.0f;

	return *this;
}

Vector4 & Vector4::operator *=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	w  = 1.0f;

	return *this;
}

Vector4 & Vector4::operator /=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	w = 1.0f;;

	return *this;
}

float Vector4::Length() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector4::LengthSqr() const
{
	return (x * x + y * y + z * z);
}


bool Vector4::operator ==(const Vector4 & rhs) const
{
	return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w));
}

Vector4 Vector4::Normalise() const
{
	return (*this / this->Length());
}

void Vector4::NormaliseSelf()
{
	*this /= this->Length();
}

std::ostream& operator<< (std::ostream& out, const Vector4& lhs)
{
	out << "(" << lhs.x << ", " << lhs.y << ", " << lhs.z << ", " << lhs.w << ")";
	return out;
}
