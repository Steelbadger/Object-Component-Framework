#include "Scale.h"
#include "Transformation.h"
#include "GameObject.h"

Scale::Scale()
{
	CalculateScaleMatrix(1, 1, 1);
}

Scale::~Scale()
{
}

void Scale::SetScale(const float x, const float y, const float z)
{
	CalculateScaleMatrix(x, y, z);
	GameObject::GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Scale::CompoundScale(const float x, const float y, const float z)
{
	D3DXMATRIX scaleMod;
	D3DXMatrixScaling(&scaleMod, x, y, z);

	scaleMatrix = scaleMatrix * scaleMod;
	GameObject::GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Scale::AdditiveScale(const float x, const float y, const float z)
{
	D3DXMATRIX scaleMod;
	D3DXMatrixScaling(&scaleMod, x, y, z);

	scaleMatrix = scaleMatrix + scaleMod;
	GameObject::GetComponent<Transformation>(GetParentID()).SetChanged();
}

D3DXMATRIX Scale::GetMatrix()
{
	return scaleMatrix;
}

void Scale::CalculateScaleMatrix(const float x, const float y, const float z)
{
	D3DXMatrixScaling(&scaleMatrix, x, y, z);
}