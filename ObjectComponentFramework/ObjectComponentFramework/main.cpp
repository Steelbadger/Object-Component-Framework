#include <iostream>
#include <D3D11.h>
#include <D3DX10math.h>

#include "Vector4.h"
#include "Hardware.h"
#include "simd.h"
#include "Quaternion.h"
#include "simdQuaternion.h"


void main()
{
	std::cout << "Running" << std::endl;

	int NumIterations = 10000000;
	const int lutSize = 10000;

	std::cout << "Running calculations with " << NumIterations << " iterations..." << std::endl << std::endl;
	D3DXVECTOR4 DXLUT[lutSize];
	Vector4 MYLUT[lutSize];
	SIMD::Floats SIMDLUT[lutSize];


	HardwareState input = HardwareState::GetInstance();
	input.Update();
	input.GetTimeForLastFrameHighResolution();
	input.Update();

	for(int i = 0; i < 1000; i++) {
		DXLUT[i] = D3DXVECTOR4(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		MYLUT[i] = Vector4(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		SIMDLUT[i] = SIMD::Floats(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
	}

	input.Update();
	input.GetTimeForLastFrameHighResolution();
	std::cout << "Generating the LUTS: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl << std::endl;

	///////////////////////////////////ADDITION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		D3DXVECTOR4 temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		D3DXVECTOR4 temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);

		D3DXVECTOR4 temp3 = temp1 + temp2;
	}

	input.Update();
	std::cout << "DirectX Maths Addition: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		Vector4 temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		Vector4 temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);

		Vector4 temp3 = temp1 + temp2;
	}

	input.Update();
	std::cout << "My Maths Addition: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		SIMD::Floats temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		SIMD::Floats temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);

		SIMD::Floats temp3 = temp1 + temp2;
	}

	input.Update();
	std::cout << "Simd Maths Addition: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;

	///////////////////////////////////LUT ADDITION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	std::cout << std::endl << "Now using LUTS" << std::endl << std::endl;
	input.Update();


	for(int i = 0; i < NumIterations; i++) {
		D3DXVECTOR4 temp3 = DXLUT[rand()%lutSize] + DXLUT[rand()%lutSize];
	}

	input.Update();
	std::cout << "DirectX Maths Addition: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		Vector4 temp3 = MYLUT[rand()%lutSize] + MYLUT[rand()%lutSize];
	}

	input.Update();
	std::cout << "My Maths Addition: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		SIMD::Floats temp3 = SIMDLUT[rand()%lutSize] + SIMDLUT[rand()%lutSize];
	}

	input.Update();
	std::cout << "Simd Maths Addition: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;

	///////////////////////////////////LENGTH\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	std::cout << std::endl << "Now Finding Length" << std::endl << std::endl;
	input.Update();


	for(int i = 0; i < NumIterations; i++) {
		D3DXVECTOR4 temp3 = DXLUT[rand()%lutSize] + DXLUT[rand()%lutSize];
		float temp4 = D3DXVec4Length(&temp3);
	}

	input.Update();
	std::cout << "DirectX Maths Length: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		Vector4 temp3 = MYLUT[rand()%lutSize] + MYLUT[rand()%lutSize];
		float temp4 = temp3.Length();
	}

	input.Update();
	std::cout << "My Maths Length: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		SIMD::Floats temp3 = SIMDLUT[rand()%lutSize] + SIMDLUT[rand()%lutSize];
		float temp4 = temp3.Length();
	}

	input.Update();
	std::cout << "Simd Maths Length: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;


	///////////////////////////////////CROSS PRODUCT\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	std::cout << std::endl << "Now Cross Product" << std::endl << std::endl;
	input.Update();


	for(int i = 0; i < NumIterations; i++) {
		D3DXVECTOR4 temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		D3DXVECTOR4 temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);

		D3DXVECTOR4 temp3;
		D3DXVec3Cross(&D3DXVECTOR3(temp3), &D3DXVECTOR3(temp3), &D3DXVECTOR3(temp3));
	}

	input.Update();
	std::cout << "DirectX Maths Cross Product: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		Vector4 temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		Vector4 temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);

		Vector4 temp3 = Cross(temp1, temp2);
	}

	input.Update();
	std::cout << "My Maths Cross Product: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		SIMD::Floats temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		SIMD::Floats temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);

		SIMD::Floats temp3 = SIMD::Cross(temp1, temp2);
	}

	input.Update();
	std::cout << "Simd Maths Cross Product: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;

	///////////////////////////////////INTERPOLATE\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	std::cout << std::endl << "Now Quaternion Interpolate" << std::endl << std::endl;
	input.Update();


	for(int i = 0; i < NumIterations; i++) {
		D3DXQUATERNION temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/10000.0f);
		D3DXQUATERNION temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/10000.0f);

		D3DXQuaternionNormalize(&temp1, &temp1);
		D3DXQuaternionNormalize(&temp2, &temp2);

		D3DXQUATERNION temp3;
		D3DXQuaternionSlerp(&temp3, &temp1, &temp2, 0.5f);
	}

	input.Update();
	std::cout << "DirectX Maths Quaternion Normalise and Slerp: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		Quaternion temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/10000.0f);
		Quaternion temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/10000.0f);

		temp1.NormalizeSelf();
		temp2.NormalizeSelf();

		Quaternion temp3 = Slerp(temp1, temp2, 0.5f);
	}

	input.Update();
	std::cout << "My Maths Quaternion Normalise and Slerp " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for(int i = 0; i < NumIterations; i++) {
		SIMD::Quaternion temp1(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/10000.0f);
		SIMD::Quaternion temp2(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/10000.0f);

		temp1.NormalizeSelf();
		temp2.NormalizeSelf();

		SIMD::Quaternion temp3 = SIMD::Slerp(temp1, temp2, 0.5f);
	}

	input.Update();
	std::cout << "SIMD Maths Quaternion Normalise and Slerp " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	D3DXQUATERNION temp1(13.2f, -2.5f, 5.1f, 8.3f);
	D3DXQUATERNION temp2(-3.2f, 0.2f, 1.2f, -9.6f);

	D3DXQuaternionNormalize(&temp1, &temp1);
	D3DXQuaternionNormalize(&temp2, &temp2);

	const float interp = 1.0f;

	D3DXQUATERNION temp3;
	D3DXQuaternionSlerp(&temp3, &temp1, &temp2, interp);
	D3DXQuaternionNormalize(&temp3, &temp3);

	Quaternion temp4(8.3f, 13.2f, -2.5f, 5.1f);
	Quaternion temp5(-9.6f, -3.2f, 0.2f, 1.2f);

	temp4.NormalizeSelf();
	temp5.NormalizeSelf();

	Quaternion temp6 = Slerp(temp4, temp5, interp);
	temp6.NormalizeSelf();

	SIMD::Quaternion temp7(8.3f, 13.2f, -2.5f, 5.1f);
	SIMD::Quaternion temp8(-9.6f, -3.2f, 0.2f, 1.2f);

	temp7.NormalizeSelf();
	temp8.NormalizeSelf();

	SIMD::Quaternion temp9 = SIMD::Slerp(temp7, temp8, interp);
	temp9.NormalizeSelf();

	std::cout << std::endl << "Compare Results: " << std::endl;
	std::cout << "(" << temp1.w << ", (" << temp1.x << ", " << temp1.y << ", " << temp1.z << ")) : " << temp4 << std::endl;
	std::cout << "(" << temp2.w << ", (" << temp2.x << ", " << temp2.y << ", " << temp2.z << ")) : " << temp5 << std::endl;

	std::cout << "DXQuat:\t(" << temp3.w << ", (" << temp3.x << ", " << temp3.y << ", " << temp3.z << "))" << std::endl;
	std::cout << "MyQuat:\t" << temp6 << std::endl << std::endl;
	std::cout << "SIMDQuat:\t" << temp9 << std::endl << std::endl;

	D3DXVECTOR3 test(1, 0, 0);
	SIMD::Floats test2(1, 0, 0, 1.0);
	Vector4 test3(1, 0, 0, 1);
	


	return;
}