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

const std::vector<ObjectID>& GameObject::GetChildren()
{
	return childGameObjects;
}

void GameObject::SetParentChild(ObjectID p, ObjectID c)
{
	Get(p).AddChild(c);
}
