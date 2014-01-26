#pragma once
///////////////////////// By Ross Davies /////////////////////////
//					Quaternion Mathematics class				//
//////////////////////////////////////////////////////////////////

#include "Matrix4x4.h"
#include "Vector4.h"

#include <iostream>


class Vector3;

class Quaternion
{
public:
	Quaternion(void);
	Quaternion(float s, float x, float y, float z);
	Quaternion(const Vector4 & rhs);
	Quaternion(const Vector3 & rhs);
	Quaternion(float s, Vector3 rhs);
	Quaternion(float s, Vector4 rhs);
	~Quaternion(void);

	void Rotation(float angle, float xComp, float yComp, float zComp);
	void Rotation(float angle, Vector3 axis);
	void Align(Vector3 v1, Vector3 v2);

	friend std::ostream& operator<< (std::ostream& out, const Quaternion& lhs);

	float Modulus() const;
	float ModulusSqr() const;

	Quaternion Conjugate() const;
	Quaternion Inverse() const;
	Quaternion Normalize() const;
	void NormalizeSelf();
	void NormalizeVector();
	float GetAngle() const;
	Vector3 GetAxis() const;
	Matrix4x4 GetRotationMatrix();

	float s, x, y, z;
};

inline Quaternion operator + (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion(q1.s + q2.s, q1.x + q2.x, q1.y + q2.y, q1.z + q2.z);
}

inline Quaternion operator - (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion(q1.s - q2.s, q1.x - q2.x, q1.y - q2.y, q1.z - q2.z);
}

inline Quaternion operator * (const Quaternion &q1,
						   const Quaternion &q2)
{
	return Quaternion	( q1.s*q2.s+q1.x*-q2.x+q1.y*-q2.y+q1.z*-q2.z
						, q1.s*q2.x+q1.x*q2.s+q1.y*q2.z+q1.z*-q2.y
						, q1.s*q2.y+q1.x*-q2.z+q1.y*q2.s+q1.z*q2.x
						, q1.s*q2.z+q1.x*q2.y+q1.y*-q2.x+q1.z*q2.s);
}
inline Quaternion operator * (const Quaternion &q,
						   const float &c)
{
	return Quaternion(q.s*c, q.x*c, q.y*c, q.z*c);
}

inline Quaternion operator * (const float &c,
						   const Quaternion &q)
{
	return Quaternion(q.s*c, q.x*c, q.y*c, q.z*c);
}

inline Quaternion operator / (const Quaternion &q,
						   const float & c)
{
	return Quaternion(q.s/c, q.x/c, q.y/c, q.z/c);
}

inline Quaternion Lerp(const Quaternion &q1, const Quaternion &q2, const float t)
{
	return ((1-t) * q1 + t * q2);
}

inline float Dot(const Quaternion &q1, const Quaternion &q2)
{
	return (q1.s*q2.s+q1.x*q2.x+q1.y*q2.y+q1.z+q2.z);
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



