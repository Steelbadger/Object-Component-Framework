#include "Orientation.h"
#include "GameObject.h"
#include "Transformation.h"
#include "ObjectManager.h"
#include <memory>

Orientation::Orientation()
{
	D3DXQuaternionIdentity(&orientation);
}

Orientation::~Orientation()
{
}

void Orientation::SetOrientation(D3DXQUATERNION in)
{
	orientation = in;
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Orientation::SetOrientation(D3DXMATRIX in)
{
	D3DXQuaternionRotationMatrix(&orientation, &in);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Orientation::Rotate(float angle, D3DXVECTOR3 axis)
{
	D3DXQUATERNION modifier;
	D3DXQuaternionRotationAxis(&modifier, &axis, angle);
	D3DXQuaternionMultiply(&orientation, &orientation, &modifier);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

void Orientation::RotateLocal(float angle, D3DXVECTOR3 axis)
{
	axis = GetRotatedPoint(axis.x, axis.y, axis.z);
	D3DXQUATERNION modifier;
	D3DXQuaternionRotationAxis(&modifier, &axis, angle);
	D3DXQuaternionMultiply(&orientation, &orientation, &modifier);
	manager->GetComponent<Transformation>(GetParentID()).SetChanged();
}

D3DXMATRIX Orientation::GetMatrix()
{
	D3DXMATRIX out;
	D3DXMatrixRotationQuaternion(&out, &orientation);
	return out;
}

D3DXVECTOR3 Orientation::GetTransformedX()
{
	return GetRotatedPoint(1,0,0);
}

D3DXVECTOR3 Orientation::GetTransformedY()
{
	return GetRotatedPoint(0,1,0);
}

D3DXVECTOR3 Orientation::GetTransformedZ()
{
	return GetRotatedPoint(0,0,1);
}

D3DXVECTOR3 Orientation::GetRotatedPoint(float x, float y, float z)
{
	D3DXVECTOR3 point(x,y,z);
	D3DXMATRIX orientationMatrix = GetMatrix();
	D3DXVec3TransformCoord(&point, &point, &orientationMatrix);
	return point;
}