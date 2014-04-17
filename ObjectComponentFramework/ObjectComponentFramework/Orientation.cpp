#include "Orientation.h"
#include "GameObject.h"
#include "Transformation.h"
#include "ObjectManager.h"
#include <memory>

Orientation::Orientation()
{
	orientation = XMQuaternionIdentity();
}

Orientation::~Orientation()
{
}

void Orientation::SetOrientation(FXMVECTOR in)
{
	orientation = in;
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Orientation::SetOrientation(CXMMATRIX in)
{
	orientation = XMQuaternionRotationMatrix(in);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Orientation::Rotate(float angle, FXMVECTOR axis)
{
	XMVECTOR modifier = XMQuaternionRotationAxis(axis, angle);
	orientation = XMQuaternionMultiply(orientation, modifier);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Orientation::RotateLocal(float angle, FXMVECTOR axis)
{
	XMVECTOR l_axis = XMVector3Rotate(axis, orientation);
	XMVECTOR modifier = XMQuaternionRotationAxis(l_axis, angle);
	orientation = XMQuaternionMultiply(orientation, modifier);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

XMMATRIX Orientation::GetMatrix()
{
	return XMMatrixRotationQuaternion(orientation);
}

XMVECTOR Orientation::GetTransformedX()
{
	static const XMVECTOR x = XMVectorSet(1.0f,0.0f,0.0f,1.0f);
	return XMVector3Transform(x, GetMatrix());
}

XMVECTOR Orientation::GetTransformedY()
{
	static const XMVECTOR y = XMVectorSet(0.0f,1.0f,0.0f,1.0f);
	return XMVector3Transform(y, GetMatrix());
}

XMVECTOR Orientation::GetTransformedZ()
{
	static const XMVECTOR z = XMVectorSet(0.0f,0.0f,1.0f,1.0f);
	return XMVector3Transform(z, GetMatrix());
}

XMVECTOR Orientation::GetRotatedPoint(float x, float y, float z)
{
	return XMVector3Transform(XMVectorSet(x, y, z, 1.0f), GetMatrix());
}