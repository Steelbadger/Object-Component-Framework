#pragma once
///////////////////////// By Ross Davies /////////////////////////
//					SIMD types and operations in a				//
//					namespace									//
//////////////////////////////////////////////////////////////////
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <iostream>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace SIMD
{
	class Integers;
	class Floats
	{
	public:
		Floats();
		Floats(float x, float y, float z, float w);
		Floats(float all);
		Floats(float* pointer);
		Floats(const Integers& ints);
		Floats(__m128 data);

		float x() const;
		float y() const;
		float z() const;
		float w() const;
		__m128 GetData() const {return data;}
		float Sum() const;
		float Length() const;

		Floats& operator += (const Floats& rhs);
		Floats& operator -= (const Floats& rhs);
		Floats& operator *= (const Floats& rhs);
		Floats& operator /= (const Floats& rhs);

		float operator [] (const int index);

		Floats operator > (const Floats& lhs) const;
		Floats operator < (const Floats& lhs) const;

		friend std::ostream& operator<< (std::ostream& out, const Floats& lhs);

		Floats& operator = (const Floats& lhs);
		bool operator == (const Floats& rhs) const;
		bool operator != (const Floats& rhs) const;

	protected:
		__m128 data;	//  a0 = x, a1 = y, a2 = z, a3 = w
	};

	Floats operator + (const Floats& lhs, const Floats& rhs);
	Floats operator - (const Floats& lhs, const Floats& rhs);
	Floats operator * (const Floats& lhs, const Floats& rhs);
	Floats operator / (const Floats& lhs, const Floats& rhs);

	Floats operator + (const Floats& lhs, const float& rhs);
	Floats operator + (const float& lhs, const Floats& rhs);

	Floats operator - (const Floats& lhs, const float& rhs);
	Floats operator - (const float& lhs, const Floats& rhs);

	Floats operator * (const Floats& lhs, const float& rhs);
	Floats operator * (const float& lhs, const Floats& rhs);

	Floats operator / (const Floats& lhs, const float& rhs);
	Floats operator / (const float& lhs, const Floats& rhs);

	Floats operator | (const Floats& lhs, const Floats& rhs);
	Floats operator ^ (const Floats& lhs, const Floats& rhs);
	Floats operator & (const Floats& lhs, const Floats& rhs);

	Floats Cross(const Floats& lhs, const Floats& rhs);
	float Dot(const Floats& lhs, const Floats& rhs);
	Floats Cosine(const Floats& values);
	Floats Sine(const Floats& values);


	class Integers
	{
	public:
		Integers();
		Integers(int x, int y, int z, int w);
		Integers(int all);
		Integers(int* pointer);
		Integers(const Floats& floats);
		Integers(__m128i data);

		int x() const;
		int y() const;
		int z() const;
		int w() const;
		__m128i GetData()const {return data;}

		Integers& operator += (const Integers& rhs);
		Integers& operator -= (const Integers& rhs);
		Integers& operator *= (const Integers& rhs);
		Integers& operator /= (const Integers& rhs);

		int operator [] (const int index);

		Integers operator > (const Integers& lhs) const;
		Integers operator < (const Integers& lhs) const;


		bool operator == (const Integers& rhs) const;
		bool operator != (const Integers& rhs) const;

	private:
		__m128i data;
	};

	Integers operator + (const Integers& lhs, const Integers& rhs);
	Integers operator - (const Integers& lhs, const Integers& rhs);
	Integers operator * (const Integers& lhs, const Integers& rhs);

	Integers operator + (const Integers& lhs, const int rhs);
	Integers operator + (const int lhs, const Integers& rhs);

	Integers operator - (const Integers& lhs, const int rhs);
	Integers operator - (const int lhs, const Integers& rhs);

	Integers operator * (const Integers& lhs, const int rhs);
	Integers operator * (const int lhs, const Integers& rhs);

	Integers operator | (const Integers& lhs, const Integers& rhs);
	Integers operator ^ (const Integers& lhs, const Integers& rhs);
	Integers operator & (const Integers& lhs, const Integers& rhs);
	Integers operator << (const Integers& lhs, const int shift);
	Integers operator >> (const Integers& lhs, const int shift);
	Integers operator ~ (const Integers& lhs);


	Floats operator + (const Floats& lhs, const Integers& rhs);
	Floats operator + (const Integers& lhs, const Floats& rhs);

	Floats operator - (const Floats& lhs, const Integers& rhs);
	Floats operator - (const Integers& lhs, const Floats& rhs);

	Floats operator * (const Floats& lhs, const Integers& rhs);
	Floats operator * (const Integers& lhs, const Floats& rhs);

	Floats operator / (const Floats& lhs, const Integers& rhs);
	Floats operator / (const Integers& lhs, const Floats& rhs);

};

