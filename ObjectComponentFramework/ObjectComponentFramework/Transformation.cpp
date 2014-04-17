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

const XMMATRIX& Transformation::GetTransformation()
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
	XMMATRIX rotation = XMMatrixIdentity();
	XMMATRIX scale = XMMatrixIdentity();
	XMMATRIX translate = XMMatrixIdentity();

	if (manager->HasComponent<Orientation>(GetParentID())) {
		rotation = manager->GetComponent<Orientation>(GetParentID()).GetMatrix();
	}
	if (manager->HasComponent<Scale>(GetParentID())) {
		scale = manager->GetComponent<Scale>(GetParentID()).GetMatrix();
	}
	if (manager->HasComponent<Position>(GetParentID())) {
		XMVECTOR pos = manager->GetComponent<Position>(GetParentID()).GetPosition();
		translate = XMMatrixTranslationFromVector(pos);
	}

	localTransform = rotation * scale * translate;
	localChanged = false;
}

void Transformation::CalculateGlobalTransformation()
{
	if (localChanged) {
		CalculateLocalTransformation();
	}
	XMMATRIX parent = XMMatrixIdentity();
	if (manager->Get(GetParentID()).HasParent()) {
		parent = manager->GetComponent<Transformation>(manager->Get(GetParentID()).GetParentID()).GetTransformation();
	}
	globalTransform = localTransform * parent;
	globalChanged = false;
}
