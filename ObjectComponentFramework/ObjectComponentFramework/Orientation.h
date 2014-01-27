#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class Orientation : public Component<Orientation>
{
public:
	Orientation();
	~Orientation();

	void SetOrientation(D3DXQUATERNION in);
	void SetOrientation(D3DXMATRIX in);
	void Rotate(float angle, D3DXVECTOR3 axis);
	void RotateLocal(float angle, D3DXVECTOR3 localAxis);
	D3DXMATRIX GetMatrix();
	D3DXVECTOR3 GetTransformedX();
	D3DXVECTOR3 GetTransformedY();
	D3DXVECTOR3 GetTransformedZ();
	D3DXVECTOR3 GetRotatedPoint(float x, float y, float z);
private:
	D3DXQUATERNION orientation;
};