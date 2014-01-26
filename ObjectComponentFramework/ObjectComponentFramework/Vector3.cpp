#include "Vector3.h"
#include "Vector4.h"


Vector3::Vector3(void):
	x(0),y(0),z(0)
{
}
Vector3::Vector3(const float _x, const float _y, const float _z)
	:x(_x),y(_y),z(_z)
{
}

Vector3::Vector3(const Vector4 &rhs):
	x(rhs.x/rhs.w), y(rhs.y/rhs.w), z(rhs.z/rhs.w)
{
}

bool Vector3::operator ==(const Vector3 & rhs) const
{
	return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
}


Vector3 & Vector3::operator +=(const Vector3 &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;

	return *this;
}

Vector3 & Vector3::operator -=(const Vector3 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;

	return *this;
}

Vector3 & Vector3::operator *=(const Vector3 &rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;

	return *this;
}

Vector3 & Vector3::operator /=(const Vector3 &rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	z /= rhs.z;

	return *this;
}


Vector3::~Vector3(void)
{
}

float Vector3::Length()
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::LengthSqr()
{
	return (x*x+y*y+z*z);
}

Vector3 Vector3::Normalize()
{
	return (*this/Length());
}

void Vector3::NormalizeSelf()
{
	*this = (*this/Length());
}

std::ostream& operator<<(std::ostream& out, Vector3& object)
{
	out << "(" << object.x << ", " << object.y << ", " << object.z << ")";
	return out;
}
