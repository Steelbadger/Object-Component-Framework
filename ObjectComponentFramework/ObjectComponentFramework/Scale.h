#pragma once

#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include <xnamath.h>

class Scale : public rabd::Component<Scale>
{
public:
	Scale();
	~Scale();

	void SetScale(const float x, const float y, const float z);
	void CompoundScale(const float x, const float y, const float z);
	void AdditiveScale(const float x, const float y, const float z);

	XMMATRIX GetMatrix();
private:
	void CalculateScaleMatrix(const float x, const float y, const float z);
	XMMATRIX scaleMatrix;
};