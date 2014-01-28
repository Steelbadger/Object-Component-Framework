#include "Transformation.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "Orientation.h"
#include "Position.h"
#include "Scale.h"


Transformation::Transformation():
	localChanged(true), globalChanged(true), viewChanged(true)
{

}

Transformation::~Transformation()
{

}

D3DXMATRIX Transformation::GetTransformation()
{
	if (globalChanged) {
		CalculateGlobalTransformation();
		viewChanged = true;
	}
	return globalTransform;
}

void Transformation::SetChanged()
{
	localChanged = true;
	SetGlobalChanged();
}

void Transformation::SetGlobalChanged()
{
	globalChanged = true;
	std::vector<ObjectID> children = GameObject::Get(GetParentID()).GetChildren();
	for (int i = 0; i < children.size(); i++) {
		GameObject::GetComponent<Transformation>(children[i]).SetGlobalChanged();
	}
}

void Transformation::CalculateLocalTransformation()
{
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	D3DXMATRIX translate;

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&translate);

	if (GameObject::HasComponent<Orientation>(GetParentID())) {
		rotation = GameObject::GetComponent<Orientation>(GetParentID()).GetMatrix();
	}
	if (GameObject::HasComponent<Scale>(GetParentID())) {
		scale = GameObject::GetComponent<Scale>(GetParentID()).GetMatrix();
	}
	if (GameObject::HasComponent<Position>(GetParentID())) {
		D3DXVECTOR3 pos = GameObject::GetComponent<Position>(GetParentID()).GetPosition();
		D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
	}

	localTransform = rotation * scale * translate;
	localChanged = false;
}

void Transformation::CalculateGlobalTransformation()
{
	if (localChanged) {
		CalculateLocalTransformation();
	}
	D3DXMATRIX parent = GameObject::GetComponent<Transformation>(GetParentID()).GetTransformation();

	globalTransform = localTransform * parent;
	globalChanged = false;
}
