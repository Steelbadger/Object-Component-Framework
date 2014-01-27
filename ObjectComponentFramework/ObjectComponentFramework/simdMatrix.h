//#pragma once
//
//#include <mmintrin.h>
//#include <xmmintrin.h>
//#include <emmintrin.h>
//#include <smmintrin.h>
//#include <iostream>
//#include "Matrix4x4.h"
//#include "simd.h"
//
//class Vector3;
//class Vector4;
//
//namespace SIMD
//{
//	class Matrix4x4
//	{
//	public:
//		Matrix4x4(void);
//		Matrix4x4(const Matrix4x4 & rhs);
//		Matrix4x4(Floats _elem[4]);
//		Matrix4x4(	float _11, float _12, float _13, float _14,
//					float _21, float _22, float _23, float _24,
//					float _31, float _32, float _33, float _34,
//					float _41, float _42, float _43, float _44);
//
//		Matrix4x4(const Quaternion &rhs);
//
//		~Matrix4x4(void);
//
//		void Translation(float x, float y, float z);
//		void Translation(const Vector3 &vec);
//		void Translation(const Vector4 &vec);
//		void RotationX(float fAngle);
//		void RotationY(float fAngle);
//		void RotationZ(float fAngle);
//		void RotationAroundVectorThroughZero(const Vector4 &Vector, float angle);
//		void Projection(float fov, float aspect, float near, float far);
//		void Transpose();
//
//		void Scale(float xScale, float yScale, float zScale);
//		void Scale(const Vector3 &scale);
//		void Scale(const Vector4 &scale);
//
//		void LookAt(const Vector4 & vFrom,
//					const Vector4 & vTo,
//					const Vector4 & vUp);
//
//		inline float operator()( unsigned int Row, unsigned int Column ) const
//		{ return elem[Row][Column]; }
//
//		inline Floats operator()( unsigned int Row ) const
//		{ return elem[Row]; }
//
//		// Output
//		friend std::ostream& operator<<(std::ostream& out, Matrix4x4& object);
//
//		static const Matrix4x4 IDENTITY;
//		static const Matrix4x4 NULLMATRIX;
//		static const double PI;
//
//	private:
//		Floats elem[4];
//	};
//
//	Matrix4x4 operator * ( const Matrix4x4 & M1, const Matrix4x4 & M2 );
//
//	Vector4 operator * ( const Matrix4x4 & M,  const Vector4 & V );
//
//	inline Matrix4x4 operator - ( const Matrix4x4 & M )
//	{
//		Floats mat[4] = {M(0), M(1), M(2), M(3)};
//
//		mat[0] = -1.0f * mat[0];
//		mat[1] = -1.0f * mat[1];
//		mat[2] = -1.0f * mat[2];
//		mat[3] = -1.0f * mat[3];
//		
//		return Matrix4x4(mat);
//	}
//
//	inline Matrix4x4 operator - ( const Matrix4x4 & M1,
//								  const Matrix4x4 & M2 )
//	{
//		Floats mat1[4] = {M1(0), M1(1), M1(2), M1(3)};
//		Floats mat2[4] = {M2(0), M2(1), M2(2), M2(3)};
//		Floats outMat[4];
//
//		outMat[0] = mat1[0] - mat2[0];
//		outMat[1] = mat1[1] - mat2[1];
//		outMat[2] = mat1[2] - mat2[2];
//		outMat[3] = mat1[3] - mat2[3];
//
//		return Matrix4x4(outMat);
//	}
//
//	inline Matrix4x4 operator + ( const Matrix4x4 & M1,
//								  const Matrix4x4 & M2 )
//	{
//		Floats mat1[4] = {M1(0), M1(1), M1(2), M1(3)};
//		Floats mat2[4] = {M2(0), M2(1), M2(2), M2(3)};
//		Floats outMat[4];
//
//		outMat[0] = mat1[0] + mat2[0];
//		outMat[1] = mat1[1] + mat2[1];
//		outMat[2] = mat1[2] + mat2[2];
//		outMat[3] = mat1[3] + mat2[3];
//
//		return Matrix4x4(outMat);
//	}
//
//	inline Matrix4x4 operator * ( const Matrix4x4 & M,
//								  const float & s )
//	{
//		Floats mat1[4] = {M(0), M(1), M(2), M(3)};
//		Floats outMat[4];
//
//		outMat[0] = mat1[0] * s;
//		outMat[1] = mat1[1] * s;
//		outMat[2] = mat1[2] * s;
//		outMat[3] = mat1[3] * s;
//
//		return Matrix4x4(outMat);
//	}
//
//	inline Matrix4x4 operator * ( const float & s,
//								  const Matrix4x4 & M )
//	{
//		Floats mat1[4] = {M(0), M(1), M(2), M(3)};
//		Floats outMat[4];
//
//		outMat[0] = mat1[0] * s;
//		outMat[1] = mat1[1] * s;
//		outMat[2] = mat1[2] * s;
//		outMat[3] = mat1[3] * s;
//
//		return Matrix4x4(outMat);
//	}
//
//	inline Matrix4x4 Transpose( Matrix4x4 const & M )
//	{
//		Floats mat[4] = {	Floats(M(0, 0), M(1, 0), M(2, 0), M(3, 0)),
//							Floats(M(0, 1), M(1, 1), M(2, 1), M(3, 1)),
//							Floats(M(0, 2), M(1, 2), M(2, 2), M(3, 2)),
//							Floats(M(0, 3), M(1, 3), M(2, 3), M(3, 3)) };
//
//		return Matrix4x4(mat);
//	}
//
//
//}