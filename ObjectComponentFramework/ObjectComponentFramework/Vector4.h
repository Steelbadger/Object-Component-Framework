#pragma once

#include <math.h>
#include <iostream>

class Vector3;
class Quaternion;

class Vector4
{
public:
	// Constructors
	Vector4(void);
	Vector4(const float _x, const float _y, const float _z, const float _w);
	Vector4(const Vector4 & rhs);
	Vector4(const Vector3 & rhs);
	Vector4(const Quaternion &rhs);

	~Vector4(void);

	// Operations with other vectors
	Vector4 & operator+=(const Vector4 & rhs);
	Vector4 & operator-=(const Vector4 & rhs);

	// Special arithmetic

	friend std::ostream& operator<< (std::ostream& out, const Vector4& lhs);

	Vector4 & operator*=(const float s);
	Vector4 & operator/=(const float s);

	bool operator==(const Vector4 & rhs) const;

	// Miscellaneous
	float Length() const;
	float LengthSqr() const;
	Vector4 Normalise() const;
	void NormaliseSelf();
	
	// Member data
	union {
		struct {float x, y, z, w;};
		struct {float r, g, b, a;};
	};
};

inline Vector4 operator + (const Vector4 &v1,
						   const Vector4 &v2)
{
	return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, 1.0f);
}

inline Vector4 operator - (const Vector4 &v1,
						   const Vector4 &v2)
{
	return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, 1.0f);
}

inline Vector4 operator - (const Vector4 &v1)
{
	return Vector4(-v1.x, -v1.y, -v1.z, 1.0f);
}

inline Vector4 operator * (const Vector4 &v,
						   const float &s)
{
	return Vector4(v.x * s, v.y * s, v.z * s, 1.0f);
}

inline Vector4 operator * (const float & s,
						   const Vector4 &v)
{
	return Vector4(v.x * s, v.y * s, v.z * s, 1.0f);
}

inline Vector4 operator / (const Vector4 &v,
						   const float & s)
{
	return Vector4(v.x / s, v.y / s, v.z / s, 1.0f);
}

inline Vector4 Cross (const Vector4 &v1,
							 const Vector4 &v2)
{
	return Vector4(v1.y * v2.z - v1.z * v2.y,
				   v1.z * v2.x - v1.x * v2.z,
				   v1.x * v2.y - v1.y * v2.x,
				   1.0f);
}

inline float Dot4(const Vector4 &v1,
						 const Vector4 &v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

inline float Dot3(const Vector4 &v1,
						 const Vector4 &v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline Vector4 Normalise (const Vector4 &v)
{
	return v / v.Length();
}

inline Vector4 CalcNormal(Vector4 pointA, Vector4 pointB, Vector4 pointC)
{
	Vector4 A(pointA);
	Vector4 B(pointB);
	Vector4 C(pointC);

	Vector4 AB = B - A;
	Vector4 AC = C - A;

	Vector4 Normal = Cross(AC, AB);
	Normal.NormaliseSelf();
	return Normal;
}

