#pragma once
//////////////////////--By Ross Davies--//////////////////////
/* A simple Vector2 maths class, with all the common
operations needed for basic vector functionality */
//////////////////////////////////////////////////////////////

#include <math.h>
#include <iostream>

class Vector2
{
public:
	// Constructors
	Vector2();
	Vector2(const float x, const float y);
	Vector2(const Vector2 & rhs);

	~Vector2();

	// Operations with other vectors
	Vector2 & operator+=(const Vector2 & rhs);
	Vector2 & operator-=(const Vector2 & rhs);

	Vector2 & operator*=(const float s);
	Vector2 & operator/=(const float s);

	bool operator==(const Vector2 & rhs) const;

	// Miscellaneous
	float Length() const;
	float LengthSqr() const;
	Vector2 Normalise();
	void NormaliseSelf();

	// Output
	friend std::ostream& operator<<(std::ostream& out, Vector2& object);

	// Member data
	union {
		struct { float x, y; };
		struct { float u, v; };
	};
};

// Normal operators for Vector arithmetic

inline Vector2 operator + (const Vector2 &v1,
							const Vector2 &v2)
{
	return Vector2(v1.x + v2.x, v1.y + v2.y);
}

inline Vector2 operator - (const Vector2 &v1,
							const Vector2 &v2)
{
	return Vector2(v1.x - v2.x, v1.y - v2.y);
}

inline Vector2 operator - (const Vector2 &v1)
{
	return Vector2(-v1.x, -v1.y);
}

inline Vector2 operator * (const Vector2 &v,
							const float &s)
{
	return Vector2(v.x * s, v.y * s);
}

inline Vector2 operator * (const float & s,
							const Vector2 &v)
{
	return Vector2(v.x * s, v.y * s);
}

inline Vector2 operator / (const Vector2 &v,
							const float & s)
{
	return Vector2(v.x / s, v.y / s);
}

inline Vector2 Normalise (const Vector2 &v)
{
	return v / v.Length();
}

inline float Dot(const Vector2 &lhs, const Vector2 &rhs)
{
	return (lhs.x * rhs.x + lhs.y * rhs.y);
}