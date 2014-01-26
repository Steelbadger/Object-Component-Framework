//////////////////////------------------//////////////////////
/*
						By Ross Davies
															*/
//////////////////////------------------//////////////////////

#include <stdio.h>
#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(const float _x, const float _y):
	x(_x),
	y(_y)
{
}

Vector2::Vector2(const Vector2 &rhs):
	x(rhs.x),
	y(rhs.y)
{
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator +=(const Vector2 &rhs)
{
	x += rhs.x;
	y += rhs.y;

	return *this;
}

Vector2& Vector2::operator -=(const Vector2 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;

	return *this;
}

Vector2& Vector2::operator *=(const float s)
{
	x *= s;
	y *= s;
	
	return *this;
}

Vector2& Vector2::operator /=(const float s)
{
	x /= s;
	y /= s;

	return *this;
}

float Vector2::Length() const
{
	return sqrt(x * x + y * y);
}

float Vector2::LengthSqr() const
{
	return (x * x + y * y);
}


bool Vector2::operator ==(const Vector2 &rhs) const
{
	return ((x == rhs.x) && (y == rhs.y));
}

Vector2 Vector2::Normalise()
{
	return (*this / this->Length());
}

void Vector2::NormaliseSelf()
{
	*this /= this->Length();
}

std::ostream& operator<<(std::ostream& out, Vector2& object)
{
	out << "(" << object.x << ", " << object.y << ")";
	return out;
}