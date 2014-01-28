#include "GameObject.h"
#include "Position.h"
#include "Orientation.h"
#include "Scale.h"
#include "Controller.h"
#include "Transformation.h"
#include "ControllerFunctors.h"
#include "ComponentManager.h"

#include <iostream>
#include <D3D11.h>
#include <D3DX10math.h>

#include "Vector4.h"
#include "Hardware.h"
#include "simd.h"
#include "Quaternion.h"
#include "simdQuaternion.h"



const int lutSize = 2;
const int NumIterations = 10000000;
const int OBJECTS = 1000;

HardwareState input = HardwareState::GetInstance();

D3DXVECTOR4 DXLUT[lutSize];
Vector4 MYLUT[lutSize];
SIMD::Floats SIMDLUT[lutSize];

void SPEEDTEST();


void main()
{
//	SPEEDTEST();

	input.Update();
	input.GetTimeForLastFrameHighResolution();
	input.Update();

	SpinController uppy;
	uppy.SetSpinSpeed(0.2f, 0.1f, 0.24f);

	ObjectID oldthingy = GameObject::New();
	GameObject* obj = &GameObject::Get(oldthingy);

	GameObject::AddComponent<Position>(oldthingy);
	GameObject::AddComponent<Orientation>(oldthingy);
	GameObject::AddComponent<Scale>(oldthingy);
	GameObject::AddComponent<Controller>(oldthingy);

	Controller* cont = &GameObject::GetComponent<Controller>(oldthingy);

	GameObject::GetComponent<Position>(oldthingy).SetPosition(2.0f, 0.0f, 2.0f);
	GameObject::GetComponent<Controller>(oldthingy).SetControlFunction<SpinController>(uppy);

	for (int i = 0; i < OBJECTS-1; i++) {
		ObjectID thingy = GameObject::New();
		GameObject::AddComponent<Position>(thingy);
		GameObject::AddComponent<Orientation>(thingy);
		GameObject::AddComponent<Scale>(thingy);
		GameObject::AddComponent<Controller>(thingy);

		GameObject::GetComponent<Position>(thingy).SetPosition(2.0f, 0.0f, 2.0f);
		GameObject::GetComponent<Controller>(thingy).SetControlFunction<SpinController>(uppy);

		GameObject::SetParentChild(oldthingy, thingy);
		oldthingy = thingy;
	}

	input.Update();
	std::cout << "Generating " << OBJECTS << " Objects: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl << std::endl;
	input.Update();


	for (int i = 0 ; i < Controller::GetList().Size() ; i++) {
		if (Controller::GetList().Exists(i)) {
			Controller* herp = &Controller::GetList().Get(i);
			Controller::GetList().Get(i).Update();
		}
	}

	for (int i = 0 ; i < Transformation::GetList().Size() ; i++) {
		if (Transformation::GetList().Exists(i)) {
			Transformation::GetList().Get(i).GetTransformation();
		}
	}

	input.Update();
	std::cout << "Updating " << OBJECTS << " Objects: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl << std::endl;
	input.Update();


	return;
}

void Addition();
void LUTAddition();
void LUTLength();
void CrossProduct();
void Interpolate();
void TestResults();
void MatrixVsQuaternion();

void SPEEDTEST()
{
	std::cout << "Running" << std::endl;

	std::cout << "Running calculations with " << NumIterations << " iterations..." << std::endl << std::endl;

	input.Update();
	input.GetTimeForLastFrameHighResolution();
	input.Update();

	for(int i = 0; i < lutSize; i++) {
		DXLUT[i] = D3DXVECTOR4(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		MYLUT[i] = Vector4(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
		SIMDLUT[i] = SIMD::Floats(float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, float(rand()%10000)/100.0f, 1.0f);
	}

	input.Update();
	input.GetTimeForLastFrameHighResolution();
	std::cout << "Generating the LUTS: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl << std::endl;

	///////////////////////////////////ADDITION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	Addition();

	///////////////////////////////////LUT ADDITION\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	LUTAddition();

	///////////////////////////////////LENGTH\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	LUTLength();

	///////////////////////////////////CROSS PRODUCT\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	CrossProduct();

	///////////////////////////////////INTERPOLATE\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

	Interpolate();


	TestResults();

	MatrixVsQuaternion();

}

void Addition()
{
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
}

void LUTAddition()
{
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
}

void LUTLength()
{
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
}

void CrossProduct()
{
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
}

void Interpolate()
{
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
}

void TestResults()
{
	float x = 6.8f;
	float y = 2.5f;
	float z = -5.3f;
	float s = 2.3f;

	Quaternion myq(s, x, y, z);
	D3DXQUATERNION dxq(x, y, z, s);
	SIMD::Quaternion simq(s, x, y, z);

	float px = 1.0f;
	float py = 2.3f;
	float pz = -4.5f;

	Vector4 myp(px, py, pz, 1.0f);
	D3DXVECTOR4 dxp(px, py, pz, 1.0f);
	Vector4 outp;

	std::cout << "Rotate: " << std::endl << myp << std::endl << "by" << std::endl << myq << std::endl << std::endl;

	myq.NormalizeSelf();
	D3DXQuaternionNormalize(&dxq, &dxq);
	simq.NormalizeSelf();

	D3DXMATRIX temp;
	D3DXMatrixRotationQuaternion(&temp, &dxq);
	D3DXVec4Transform(&dxp, &dxp, &temp);
	dxp = dxp/dxp.w;

	std::cout << "DirectX: (" << dxp.x << ", " << dxp.y << ", " << dxp.z << ", " << dxp.w << ")" << std::endl;

	outp = myq * Quaternion(myp) * myq.Inverse();

	std::cout << "MyMaths: " << outp << std::endl;

	outp = simq * SIMD::Quaternion(myp) * simq.Inverse();

	std::cout << "SimMath: " << outp << std::endl << std::endl;
}

void MatrixVsQuaternion()
{
	float x = 6.8f;
	float y = 2.5f;
	float z = -5.3f;
	float s = 2.3f;

	Quaternion myq(s, x, y, z);
	D3DXQUATERNION dxq(x, y, z, s);
	SIMD::Quaternion simq(s, x, y, z);

	std::cout << std::endl << "Now Quaternion Vs Matrix" << std::endl << std::endl;
	input.Update();

	for (int i = 0 ; i < NumIterations; i++) {
		D3DXVECTOR4 temp = DXLUT[rand()%lutSize];
		D3DXQUATERNION point(temp.x, temp.y, temp.z, 0.0f);
		D3DXQUATERNION dxqinv;
		D3DXQuaternionInverse(&dxqinv, &dxq);
		
		D3DXQuaternionMultiply(&point, &dxq, &point);
		D3DXQuaternionMultiply(&point, &point, &dxqinv);
	}

	input.Update();
	std::cout << "DirectX Maths Quaternion Multiplication: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();



	for (int i = 0 ; i < NumIterations; i++) {
		D3DXVECTOR4 temp = DXLUT[rand()%lutSize];
		D3DXVECTOR4 point(temp.x, temp.y, temp.z, 1.0f);
		D3DXMATRIX dxqmat;
		D3DXMatrixRotationQuaternion(&dxqmat, &dxq);
		
		D3DXVec4Transform(&point, &point, &dxqmat);
		point = point/point.w;
	}

	input.Update();
	std::cout << "DirectX Maths Quaternion Matrix Multiplication: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for (int i = 0 ; i < NumIterations; i++) {
		D3DXVECTOR4 temp = DXLUT[rand()%lutSize];
		Vector4 point(temp.x, temp.y, temp.z, 1.0f);
		point = myq * Quaternion(point) * myq.Inverse();
	}

	input.Update();
	std::cout << "My Maths Quaternion Multiplication: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();


	for (int i = 0 ; i < NumIterations; i++) {
		D3DXVECTOR4 temp = DXLUT[rand()%lutSize];
		Vector4 point(temp.x, temp.y, temp.z, 1.0f);
		Matrix4x4 mat = myq.GetRotationMatrix();
		point = mat * point;
		point = point/point.w;
	}

	input.Update();
	std::cout << "My Maths Matrix Multiplication: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();

	for (int i = 0 ; i < NumIterations; i++) {
		SIMD::Quaternion temp = SIMDLUT[rand()%lutSize];
		temp = simq * temp * simq.Inverse();
	}

	input.Update();
	std::cout << "My SIMD Maths Quaternion Multiplication: " << input.GetTimeForLastFrameHighResolution() << "s" << std::endl;
	input.Update();


}