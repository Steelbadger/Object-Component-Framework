#include "Scale.h"
#include "Transformation.h"
#include "GameObject.h"
#include "ObjectManager.h"

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
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Scale::CompoundScale(const float x, const float y, const float z)
{
	XMMATRIX scaleMod = XMMatrixScaling(x, y, z);
	scaleMatrix = scaleMatrix * scaleMod;
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Scale::AdditiveScale(const float x, const float y, const float z)
{
	XMVECTOR prevScale = XMVector3Transform(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f), scaleMatrix);
	scaleMatrix = XMMatrixScalingFromVector(prevScale + XMVectorSet(x, y, z, 1.0f));
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

XMMATRIX Scale::GetMatrix()
{
	return scaleMatrix;
}

void Scale::CalculateScaleMatrix(const float x, const float y, const float z)
{
	scaleMatrix = XMMatrixScaling(x, y, z);
}