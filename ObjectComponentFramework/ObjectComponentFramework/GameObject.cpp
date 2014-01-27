#include "GameObject.h"

#include "Position.h"
#include "Orientation.h"

GameObject::GameObject()
{
	modified = true;
}

GameObject::~GameObject()
{
}

ObjectID GameObject::GetComponentOfType(ComponentType type)
{
	return components[type];
}

//D3DXMATRIX GameObject::GetLocalMatrix()
//{
//	if (modified == true) {
//		D3DXMATRIX position;
//		D3DXMATRIX orientation;
//		D3DXMATRIX parent;
//
//		D3DXMatrixIdentity(&localMatrix);
//		D3DXMatrixIdentity(&position);
//		D3DXMatrixIdentity(&orientation);
//		D3DXMatrixIdentity(&parent);
//
//		GameObject* thingy = this;
//
//		D3DXVECTOR3 pos(0,0,0);
//		if (HasComponent<Position>()) {
//			pos = GameObject::GetComponent<Position>().GetPosition();	
//		}
//		if (HasComponent<Orientation>()) {
//			orientation = GameObject::GetComponent<Orientation>().GetMatrix();
//		}
//		if (HasParent()) {
//			parent = GameObject::Get(GetParentID()).GetLocalMatrix();
//			pos = GameObject::GetComponent<Orientation>(GetParentID()).GetRotatedPoint(pos.x, pos.y, pos.z);
//			pos = pos + GameObject::GetComponent<Position>(GetParentID()).GetPosition();
//		}
//
//		D3DXMatrixTranslation(&position, pos.x, pos.y, pos.z);
//
//		D3DXMatrixMultiply(&localMatrix, &parent, &orientation);
//		D3DXMatrixMultiply(&localMatrix, &orientation, &position);
//		modified = false;
//	}
//	return localMatrix;
//}

void GameObject::HasBeenModified()
{
	modified = true;
	for (int i = 0; i < childGameObjects.size(); i++) {
		GameObject::Get(childGameObjects[i]).HasBeenModified();
	}
}

void GameObject::AddChild(ObjectID id)
{
	for (std::vector<ObjectID>::iterator it = childGameObjects.begin(); it != childGameObjects.end(); it++) {
		if (*it == id) {
			Error("Duplicate child, not added");
			//duplicate, panic!
			return;
		}
	}
	childGameObjects.push_back(id);
	GameObject::Get(id).SetParentID(GetID());
}

void GameObject::AddComponent(ObjectID id, ComponentType type)
{
	if (components.count(type) == 0) {
		components[type] = id;
	} else {
		Error("Duplicate component, not added");
		//panic
	} 
}

void GameObject::RemoveComponent(ComponentType type)
{
	if (components.count(type) != 0) {
		components.erase(type);
	} else {
		Error("Cannot remove component, component does not exist");
		//panic
	}
}

void GameObject::RemoveChild(ObjectID id)
{
	for (std::vector<ObjectID>::iterator it = childGameObjects.begin(); it != childGameObjects.end(); it++) {
		if (*it == id) {
			Get(id).SetParentID(-1);
			childGameObjects.erase(it);
			return;
		}
	}
	Error("Specified object is not a child of this object");
	//panic
}

std::vector<ObjectID> GameObject::GetChildren()
{
	return childGameObjects;
}

void GameObject::SetParentChild(ObjectID p, ObjectID c)
{
	Get(p).AddChild(c);
}
