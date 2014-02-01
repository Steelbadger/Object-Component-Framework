#include "Transformation.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "Orientation.h"
#include "Position.h"
#include "Scale.h"
#include "simdUtility.h"


Transformation::Transformation():
	localChanged(true), globalChanged(true), viewChanged(true)
{

}

Transformation::~Transformation()
{

}

const D3DXMATRIX& Transformation::GetTransformation()
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
	viewChanged = true;
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

	//localTransform = D3DXMATRIX(&SIMD::Multiply(Matrix4x4(rotation), Matrix4x4(scale))(0,0));
	//localTransform = D3DXMATRIX(&SIMD::Multiply(Matrix4x4(localTransform), Matrix4x4(translate))(0,0));

	localTransform = rotation * scale * translate;
	localChanged = false;
}

void Transformation::CalculateGlobalTransformation()
{
	if (localChanged) {
		CalculateLocalTransformation();
	}
	D3DXMATRIX parent;
	D3DXMatrixIdentity(&parent);
	if (GameObject::Get(GetParentID()).HasParent()) {
		parent = GameObject::GetComponent<Transformation>(GameObject::Get(GetParentID()).GetParentID()).GetTransformation();
	}
	//globalTransform = D3DXMATRIX(&SIMD::Multiply(Matrix4x4(localTransform), Matrix4x4(parent))(0,0));
	globalTransform = localTransform * parent;
	globalChanged = false;
}
