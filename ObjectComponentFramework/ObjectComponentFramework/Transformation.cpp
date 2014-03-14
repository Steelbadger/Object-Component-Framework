#include "Transformation.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "Orientation.h"
#include "Position.h"
#include "Scale.h"
#include "simdUtility.h"
#include "ObjectManager.h"
#include "LookupTable.h"


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
	std::vector<rabd::ObjectID>* children = &manager->Get(GetParentID()).GetChildren();
	for (int i = 0; i < children->size(); i++) {
		manager->GetComponent<Transformation>(children->at(i)).SetGlobalChanged();
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

	if (manager->HasComponent<Orientation>(GetParentID())) {
		rotation = manager->GetComponent<Orientation>(GetParentID()).GetMatrix();
	}
	if (manager->HasComponent<Scale>(GetParentID())) {
		scale = manager->GetComponent<Scale>(GetParentID()).GetMatrix();
	}
	if (manager->HasComponent<Position>(GetParentID())) {
		D3DXVECTOR3 pos = manager->GetComponent<Position>(GetParentID()).GetPosition();
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
	if (manager->Get(GetParentID()).HasParent()) {
		parent = manager->GetComponent<Transformation>(manager->Get(GetParentID()).GetParentID()).GetTransformation();
	}
	//globalTransform = D3DXMATRIX(&SIMD::Multiply(Matrix4x4(localTransform), Matrix4x4(parent))(0,0));
	globalTransform = localTransform * parent;
	globalChanged = false;
}
