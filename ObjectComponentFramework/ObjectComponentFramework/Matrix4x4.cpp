#include "Matrix4x4.h"

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

#include <memory.h>
#include <stdio.h>
#include <math.h>

const Matrix4x4 Matrix4x4::IDENTITY(1.0f,0.0f,0.0f,0.0f,
									0.0f,1.0f,0.0f,0.0f,
									0.0f,0.0f,1.0f,0.0f,
									0.0f,0.0f,0.0f,1.0f);

const Matrix4x4 Matrix4x4::NULLMATRIX(0.0f,0.0f,0.0f,0.0f,
									  0.0f,0.0f,0.0f,0.0f,
									  0.0f,0.0f,0.0f,0.0f,
									  0.0f,0.0f,0.0f,0.0f);

const double Matrix4x4::PI = 3.1415926535897932384626433832795;

Matrix4x4::Matrix4x4(void)
{
}

Matrix4x4::Matrix4x4(const Quaternion &rhs)
{
	float angle = acos(rhs.s)*2;
	float m = sin(angle/2);
	Vector4 RotationVector = Vector4(rhs.x, rhs.y, rhs.z, 1.0f)/m;
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = cos(angle);
	float c1 = 1-c;
	float s = sin(angle);
	float a = RotationVector.x;
	float b = RotationVector.y;
	float g = RotationVector.z;
	
	elem[0][0] = a*a*c1+c;
	elem[1][0] = a*b*c1-g*s;
	elem[2][0] = a*g*c1+b*s;
	elem[0][1] = a*b*c1+g*s;
	elem[1][1] = b*b*c1+c;
	elem[2][1] = b*g*c1-a*s;
	elem[0][2] = a*g*c1-b*s;
	elem[1][2] = b*g*c1+a*s;
	elem[2][2] = g*g*c1+c;	
}

Matrix4x4::~Matrix4x4(void)
{
}

Matrix4x4::Matrix4x4(const Matrix4x4 & rhs)
{
	memcpy(elem, rhs.elem, sizeof(float) * 16);
}

Matrix4x4::Matrix4x4(float _11, float _12, float _13, float _14,
					 float _21, float _22, float _23, float _24,
					 float _31, float _32, float _33, float _34,
					 float _41, float _42, float _43, float _44)
{
	elem[0][0] = _11;	elem[0][1] = _12;	elem[0][2] = _13;	elem[0][3] = _14;
	elem[1][0] = _21;	elem[1][1] = _22;	elem[1][2] = _23;	elem[1][3] = _24;
	elem[2][0] = _31;	elem[2][1] = _32;	elem[2][2] = _33;	elem[2][3] = _34;
	elem[3][0] = _41;	elem[3][1] = _42;	elem[3][2] = _43;	elem[3][3] = _44;
}

Matrix4x4::Matrix4x4(float _elem[][4])
{
	memcpy(elem, _elem, sizeof(float) * 16);
}

Matrix4x4 operator*( const Matrix4x4 &M1,
                     const Matrix4x4 &M2)
{
    Matrix4x4 ret;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            float Value = 0;
          
            for(int k = 0; k < 4; k++)
            {
                Value += M1(i,k) * M2(k,j);
            }

            ret(i,j) = Value;
        }
    }

    return ret;
}

Vector4 operator * ( const Matrix4x4 &M,
					 const Vector4 &V )
{
    Vector4 ret;

	ret.x = M(0,0) * V.x + M(1,0) * V.y + M(2,0) * V.z + M(3,0) * V.w;
	ret.y = M(0,1) * V.x + M(1,1) * V.y + M(2,1) * V.z + M(3,1) * V.w;
	ret.z = M(0,2) * V.x + M(1,2) * V.y + M(2,2) * V.z + M(3,2) * V.w;
	ret.w = M(0,3) * V.x + M(1,3) * V.y + M(2,3) * V.z + M(3,3) * V.w;

    return ret;
}

void Matrix4x4::Translation(float X, float Y, float Z)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);

	elem[3][0] = X;
	elem[3][1] = Y;
	elem[3][2] = Z;
}

void Matrix4x4::Translation(const Vector3 &vec)
{
	Translation(vec.x, vec.y, vec.z);
}

void Matrix4x4::Translation(const Vector4 &vec)
{
	Translation(vec.x, vec.y, vec.z);
}

void Matrix4x4::RotationX(float fAngle)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = cos(fAngle);
	float s = sin(fAngle);
	elem[1][1] = c;
	elem[1][2] = s;
	elem[2][1] = -s;
	elem[2][2] = c;
}

void Matrix4x4::RotationY(float fAngle)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = cos(fAngle);
	float s = sin(fAngle);
	elem[0][0] = c;
	elem[2][0] = s;
	elem[0][2] = -s;
	elem[2][2] = c;
}

void Matrix4x4::RotationZ(float fAngle)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = cos(fAngle);
	float s = sin(fAngle);
	elem[0][0] = c;
	elem[0][1] = s;
	elem[1][0] = -s;
	elem[1][1] = c;
}

void Matrix4x4::RotationAroundVectorThroughZero(const Vector4 &Vector, float angle)
{
	Vector4 RotationVector = Vector.Normalise();
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	float c = cos(angle);
	float c1 = 1-c;
	float s = sin(angle);
	float a = RotationVector.x;
	float b = RotationVector.y;
	float g = RotationVector.z;
	
	elem[0][0] = a*a*c1+c;
	elem[1][0] = a*b*c1-g*s;
	elem[2][0] = a*g*c1+b*s;
	elem[0][1] = a*b*c1+g*s;
	elem[1][1] = b*b*c1+c;
	elem[2][1] = b*g*c1-a*s;
	elem[0][2] = a*g*c1-b*s;
	elem[1][2] = b*g*c1+a*s;
	elem[2][2] = g*g*c1+c;
}

void Matrix4x4::Scale(float xf, float yf, float zf)
{
	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);

	elem[0][0] = xf;
	elem[1][1] = yf;
	elem[2][2] = zf;
}

void Matrix4x4::Scale(const Vector3 &vec)
{
	Scale(vec.x, vec.y, vec.z);
}

void Matrix4x4::Scale(const Vector4 &vec)
{
	Scale(vec.x, vec.y, vec.z);
}

void Matrix4x4::LookAt(const Vector4 & vFrom, const Vector4 & vTo, const Vector4 & vUp)
{
	Vector4 vZ = Normalise(vFrom - vTo);
	Vector4 vX = Normalise(Cross(vUp, vZ));
	Vector4 vY = Cross(vZ, vX);

    elem[0][0] = vX.x;	elem[0][1] = vY.x;	elem[0][2] = vZ.x;	elem[0][3] = 0;
	elem[1][0] = vX.y;	elem[1][1] = vY.y;	elem[1][2] = vZ.y;	elem[1][3] = 0;
	elem[2][0] = vX.z;	elem[2][1] = vY.z;	elem[2][2] = vZ.z;	elem[2][3] = 0;

	elem[3][0] = Dot3(-vX, vFrom);
	elem[3][1] = Dot3(-vY, vFrom);
	elem[3][2] = Dot3(-vZ, vFrom);
	elem[3][3] = 1;
}

void Matrix4x4::Transpose()
{
	Matrix4x4 temp;

	temp.elem[0][0] = elem[0][0];
	temp.elem[1][0] = elem[0][1];
	temp.elem[2][0] = elem[0][2];
	temp.elem[3][0] = elem[0][3];
	temp.elem[0][1] = elem[1][0];
	temp.elem[1][1] = elem[1][1];
	temp.elem[2][1] = elem[1][2];
	temp.elem[3][1] = elem[1][3];
	temp.elem[0][2] = elem[2][0];
	temp.elem[1][2] = elem[2][1];
	temp.elem[2][2] = elem[2][2];
	temp.elem[3][2] = elem[2][3];
	temp.elem[0][3] = elem[3][0];
	temp.elem[1][3] = elem[3][1];
	temp.elem[2][3] = elem[3][2];
	temp.elem[3][3] = elem[3][3];

	memcpy(elem, temp.elem, sizeof(float) * 16);
}

void Matrix4x4::Projection(float fov, float aspect, float near, float far)
{

	memcpy(elem, NULLMATRIX.elem, sizeof(float) * 16);

	float h = 1.0f/tan(fov*PI/360);
	float neg_depth = near-far;

	elem[0][0] = h / aspect;
	elem[1][1] = h;
	elem[2][2] = (far + near)/neg_depth;
	elem[2][3] = -1;
	elem[3][2] = 2.0f*(near*far)/neg_depth;

}

std::ostream& operator<<(std::ostream& out, Matrix4x4& object)
{
	for (int i = 0; i < 4; i++) {
		out << "|";
		for (int j = 0; j < 3; j++) {
			out << object(i,j) << ", ";
		}
		out << object(i,4) << "|" << std::endl;
	}
	return out;
}
