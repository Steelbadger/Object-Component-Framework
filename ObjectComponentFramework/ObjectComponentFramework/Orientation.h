#pragma once
#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <xnamath.h>

class Orientation : public rabd::Component<Orientation>
{
public:
	Orientation();
	~Orientation();

	void SetOrientation(FXMVECTOR in);
	void SetOrientation(CXMMATRIX in);
	void Rotate(float angle, FXMVECTOR axis);
	void RotateLocal(float angle, FXMVECTOR localAxis);
	XMMATRIX GetMatrix();
	XMVECTOR GetTransformedX();
	XMVECTOR GetTransformedY();
	XMVECTOR GetTransformedZ();
	XMVECTOR GetRotatedPoint(float x, float y, float z);
	XMVECTOR GetRotatedPoint(XMVECTOR v);
private:
	XMVECTOR orientation;
};