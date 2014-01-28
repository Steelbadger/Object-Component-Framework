#pragma once


#include "Component.h"
#include <D3D11.h>
#include <D3DX10math.h>

class Transformation : public Component<Transformation>
{
public:
	Transformation();
	~Transformation();

	D3DXMATRIX GetTransformation();
	void SetChanged();
private:
	void CalculateLocalTransformation();
	void CalculateGlobalTransformation();
	void SetGlobalChanged();

	bool localChanged;
	bool globalChanged;

	D3DXMATRIX localTransform;
	D3DXMATRIX globalTransform;
};