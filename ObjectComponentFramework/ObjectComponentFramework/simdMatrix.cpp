#include "simdMatrix.h"

#include <memory.h>
#include <stdio.h>
#include <math.h>

namespace SIMD
{
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

	Matrix4x4::~Matrix4x4(void)
	{
	}

	Matrix4x4::Matrix4x4(float _11, float _12, float _13, float _14,
						 float _21, float _22, float _23, float _24,
						 float _31, float _32, float _33, float _34,
						 float _41, float _42, float _43, float _44)
	{
		rows[0] = _mm_set_ps(_11, _12, _13, _14);
		rows[1] = _mm_set_ps(_21, _22, _23, _24);
		rows[2] = _mm_set_ps(_31, _32, _33, _34);
		rows[3] = _mm_set_ps(_41, _42, _43, _44);
	}

	Matrix4x4::Matrix4x4(const float* data)
	{
		for (int i = 0; i < 4; i++) {
			rows[i] = _mm_set_ps(data[i*4], data[i*4+1], data[i*4+2], data[i*4+3]);
		}
	}

	Matrix4x4::Matrix4x4(const __m128* data)
	{
		for (int i = 0; i < 4; i++) {
			rows[i] = data[i];
		}
	}

	Matrix4x4::Matrix4x4(const __m128& row0, const __m128& row1, const __m128& row2, const __m128& row3)
	{
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
		rows[3] = row3;
	}


	//Matrix4x4::Matrix4x4(float _elem[][4])
	//{
	//	memcpy(elem, _elem, sizeof(float) * 16);
	//}

	//Matrix4x4 operator*( const Matrix4x4 &M1,
	//					 const Matrix4x4 &M2)
	//{
	//	Matrix4x4 ret;

	//	for(int i = 0; i < 4; i++)
	//	{
	//		for(int j = 0; j < 4; j++)
	//		{
	//			float Value = 0;
 //         
	//			for(int k = 0; k < 4; k++)
	//			{
	//				Value += M1(i,k) * M2(k,j);
	//			}

	//			ret(i,j) = Value;
	//		}
	//	}

	//	return ret;
	//}

	//Vector4 operator * ( const Matrix4x4 &M,
	//					 const Vector4 &V )
	//{
	//	Vector4 ret;

	//	ret.x = M(0,0) * V.x + M(1,0) * V.y + M(2,0) * V.z + M(3,0) * V.w;
	//	ret.y = M(0,1) * V.x + M(1,1) * V.y + M(2,1) * V.z + M(3,1) * V.w;
	//	ret.z = M(0,2) * V.x + M(1,2) * V.y + M(2,2) * V.z + M(3,2) * V.w;
	//	ret.w = M(0,3) * V.x + M(1,3) * V.y + M(2,3) * V.z + M(3,3) * V.w;

	//	return ret;
	//}

	//void Matrix4x4::Translation(float X, float Y, float Z)
	//{
	//	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);

	//	elem[3][0] = X;
	//	elem[3][1] = Y;
	//	elem[3][2] = Z;
	//}

	//void Matrix4x4::Translation(const Vector3 &vec)
	//{
	//	Translation(vec.x, vec.y, vec.z);
	//}

	//void Matrix4x4::Translation(const Vector4 &vec)
	//{
	//	Translation(vec.x, vec.y, vec.z);
	//}

	//void Matrix4x4::RotationX(float fAngle)
	//{
	//	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	//	float c = cos(fAngle);
	//	float s = sin(fAngle);
	//	elem[1][1] = c;
	//	elem[1][2] = s;
	//	elem[2][1] = -s;
	//	elem[2][2] = c;
	//}

	//void Matrix4x4::RotationY(float fAngle)
	//{
	//	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	//	float c = cos(fAngle);
	//	float s = sin(fAngle);
	//	elem[0][0] = c;
	//	elem[2][0] = s;
	//	elem[0][2] = -s;
	//	elem[2][2] = c;
	//}

	//void Matrix4x4::RotationZ(float fAngle)
	//{
	//	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	//	float c = cos(fAngle);
	//	float s = sin(fAngle);
	//	elem[0][0] = c;
	//	elem[0][1] = s;
	//	elem[1][0] = -s;
	//	elem[1][1] = c;
	//}

	//void Matrix4x4::RotationAroundVectorThroughZero(const Vector4 &Vector, float angle)
	//{
	//	Vector4 RotationVector = Vector.Normalise();
	//	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);
	//	float c = cos(angle);
	//	float c1 = 1-c;
	//	float s = sin(angle);
	//	float a = RotationVector.x;
	//	float b = RotationVector.y;
	//	float g = RotationVector.z;
	//
	//	elem[0][0] = a*a*c1+c;
	//	elem[1][0] = a*b*c1-g*s;
	//	elem[2][0] = a*g*c1+b*s;
	//	elem[0][1] = a*b*c1+g*s;
	//	elem[1][1] = b*b*c1+c;
	//	elem[2][1] = b*g*c1-a*s;
	//	elem[0][2] = a*g*c1-b*s;
	//	elem[1][2] = b*g*c1+a*s;
	//	elem[2][2] = g*g*c1+c;
	//}

	//void Matrix4x4::Scale(float xf, float yf, float zf)
	//{
	//	memcpy(elem, IDENTITY.elem, sizeof(float) * 16);

	//	elem[0][0] = xf;
	//	elem[1][1] = yf;
	//	elem[2][2] = zf;
	//}

	//void Matrix4x4::Scale(const Vector3 &vec)
	//{
	//	Scale(vec.x, vec.y, vec.z);
	//}

	//void Matrix4x4::Scale(const Vector4 &vec)
	//{
	//	Scale(vec.x, vec.y, vec.z);
	//}

	//void Matrix4x4::LookAt(const Vector4 & vFrom, const Vector4 & vTo, const Vector4 & vUp)
	//{
	//	Vector4 vZ = Normalise(vFrom - vTo);
	//	Vector4 vX = Normalise(Cross(vUp, vZ));
	//	Vector4 vY = Cross(vZ, vX);

	//	elem[0][0] = vX.x;	elem[0][1] = vY.x;	elem[0][2] = vZ.x;	elem[0][3] = 0;
	//	elem[1][0] = vX.y;	elem[1][1] = vY.y;	elem[1][2] = vZ.y;	elem[1][3] = 0;
	//	elem[2][0] = vX.z;	elem[2][1] = vY.z;	elem[2][2] = vZ.z;	elem[2][3] = 0;

	//	elem[3][0] = Dot3(-vX, vFrom);
	//	elem[3][1] = Dot3(-vY, vFrom);
	//	elem[3][2] = Dot3(-vZ, vFrom);
	//	elem[3][3] = 1;
	//}

	//void Matrix4x4::Transpose()
	//{
	//	Matrix4x4 temp;

	//	temp.elem[0][0] = elem[0][0];
	//	temp.elem[1][0] = elem[0][1];
	//	temp.elem[2][0] = elem[0][2];
	//	temp.elem[3][0] = elem[0][3];
	//	temp.elem[0][1] = elem[1][0];
	//	temp.elem[1][1] = elem[1][1];
	//	temp.elem[2][1] = elem[1][2];
	//	temp.elem[3][1] = elem[1][3];
	//	temp.elem[0][2] = elem[2][0];
	//	temp.elem[1][2] = elem[2][1];
	//	temp.elem[2][2] = elem[2][2];
	//	temp.elem[3][2] = elem[2][3];
	//	temp.elem[0][3] = elem[3][0];
	//	temp.elem[1][3] = elem[3][1];
	//	temp.elem[2][3] = elem[3][2];
	//	temp.elem[3][3] = elem[3][3];

	//	memcpy(elem, temp.elem, sizeof(float) * 16);
	//}

	//void Matrix4x4::Projection(float fov, float aspect, float near, float far)
	//{

	//	memcpy(columns, NULLMATRIX.columns, sizeof(float) * 16);

	//	float h = 1.0f/tan(fov*PI/360);
	//	float neg_depth = near-far;

	//	columns[0][0] = h / aspect;
	//	elem[1][1] = h;
	//	elem[2][2] = (far + near)/neg_depth;
	//	elem[2][3] = -1;
	//	elem[3][2] = 2.0f*(near*far)/neg_depth;

	//}

	//std::ostream& operator<<(std::ostream& out, Matrix4x4& object)
	//{
	//	for (int i = 0; i < 4; i++) {
	//		out << "|";
	//		for (int j = 0; j < 3; j++) {
	//			out << object(i,j) << ", ";
	//		}
	//		out << object(i,4) << "|" << std::endl;
	//	}
	//	return out;
	//}

	Matrix4x4 Perspective(const float fov, const float aspect, const float znear, const float zfar)
	{
		float yScale = 1.0f / tan(fov/2);
		float xScale = yScale/aspect;
		return Matrix4x4(	xScale,		0.0f,		0.0f,						0.0f,
							0.0f,		yScale,		0.0f,						0.0f,
							0.0f,		0.0f,		zfar/(zfar-znear),			1.0f,
							0.0f,		0.0f,		-znear*zfar/(zfar-znear),	0.0f);

	}

	Matrix4x4 Orthographic(const float width, const float height, const float znear, const float zfar)
	{
		return Matrix4x4(	2/width,	0.0f,		0.0f,						0.0f,
							0.0f,		2/height,	0.0f,						0.0f,
							0.0f,		0.0f,		1/(zfar-znear),				0.0f,
							0.0f,		0.0f,		-znear/(zfar-znear),		1.0f);
	}

	Matrix4x4 Inverse(const Matrix4x4& in)
	{
		//  Main body of algorithm found at http://download.intel.com/design/PentiumIII/sml/24504301.pdf
		//  Modified slightly to work with this implementation

		//  DOESN'T WORK!!!!

		__m128 minor0, minor1, minor2, minor3;
		__m128 row0, row1, row2, row3;
		__m128 det, tmp1;

		row0 = in.GetRow(3);
		row1 = in.GetRow(2);
		row2 = in.GetRow(1);
		row3 = in.GetRow(0);

		_MM_TRANSPOSE4_PS(row3, row2, row1, row0);
		row1 = _mm_shuffle_ps(row1, row1, _MM_SHUFFLE(1,0,3,2));
		row3 = _mm_shuffle_ps(row3, row3, _MM_SHUFFLE(1,0,3,2));
		// -----------------------------------------------
		tmp1   = _mm_mul_ps(row2, row3);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0  = _mm_mul_ps(row1, tmp1);
		minor1  = _mm_mul_ps(row0, tmp1);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0  = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
		minor1  = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
		minor1  = _mm_shuffle_ps(minor1, minor1, 0x4E);
		//    -----------------------------------------------
		tmp1   = _mm_mul_ps(row1, row2);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor0  = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
		minor3  = _mm_mul_ps(row0, tmp1);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0  = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
		minor3  = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
		minor3  = _mm_shuffle_ps(minor3, minor3, 0x4E);
		//    -----------------------------------------------
		tmp1   = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		row2   = _mm_shuffle_ps(row2, row2, 0x4E);
		minor0  = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
		minor2  = _mm_mul_ps(row0, tmp1);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor0  = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
		minor2  = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
		minor2  = _mm_shuffle_ps(minor2, minor2, 0x4E);
		//    -----------------------------------------------
		tmp1   = _mm_mul_ps(row0, row1);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
		//           -----------------------------------------------
		tmp1   = _mm_mul_ps(row0, row3);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
		minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
		minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
		//           -----------------------------------------------
		tmp1   = _mm_mul_ps(row0, row2);
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
		minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
		minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
		tmp1   = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
		minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
		minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
 
		// Evaluation of determinant and its reciprocal value. In the original Intel document,
		// 1/det was evaluated using a fast rcpps command with subsequent approximation using
		// the Newton-Raphson algorithm. Here, we go for a IEEE-compliant division instead,
		// so as to not compromise precision at all.
		det    = _mm_mul_ps(row0, minor0);
		det    = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
		det    = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
		det    = _mm_div_ss(_mm_set_ss(1.0f), det); // <--- yay, one original line not copied from Intel
		det    = _mm_shuffle_ps(det, det, 0x00);
		// warning, Intel's variable naming is very confusing: now 'det' is 1/det !

		minor0 = _mm_mul_ps(det, minor0);
		minor1 = _mm_mul_ps(det, minor1);
		minor2 = _mm_mul_ps(det, minor2);
		minor3 = _mm_mul_ps(det, minor3);

		minor0 = _mm_shuffle_ps(minor0, minor0, _MM_SHUFFLE(0,1,2,3));
		minor1 = _mm_shuffle_ps(minor1, minor1, _MM_SHUFFLE(0,1,2,3));
		minor2 = _mm_shuffle_ps(minor2, minor2, _MM_SHUFFLE(0,1,2,3));
		minor3 = _mm_shuffle_ps(minor3, minor3, _MM_SHUFFLE(0,1,2,3));

		return Matrix4x4(minor0, minor1, minor2, minor3);
	}
}
