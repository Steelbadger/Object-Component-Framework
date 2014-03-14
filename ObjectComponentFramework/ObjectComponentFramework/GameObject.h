/*
	File:		GameObject.h
	Version:	2.1
	Date:		9th March 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	GameObject
	Requires:	rabd::Component

	Description:
	A GameObject is a special kind of component that can have multiple other components as children
*/


#pragma once
#include "Component.h"
#include "LookupTable.h"

#include <vector>
#include <iostream>

namespace rabd
{
class GameObject : public Component<GameObject>
{
public:
	//  Default Constructor
	GameObject(){};

	//  Default Destructor
	~GameObject(){};

	//  Assign the GameObject a child of the templated type, object referenced by passed id
	//  This function does not alter the component, so the new child should be modified separately
	//  to reference this object as its parent.
	template<class T>
	void AssignChild(ObjectID id) {
		int typeID = T::GetTypeID();
		if (typeID != GetTypeID()) {
			if (components.size() <= typeID) {
				components.resize(typeID+1, -1);
			}
			components[typeID] = id;
		} else {
			std::cout << typeid(T).name() << " cannot be added as a component, must be a child" << std::endl;
		}
	}

	template<class T>
	inline bool HasComponent() const {
		return ((components.size() > T::GetTypeID()) && (components[T::GetTypeID()] >= 0));
	}

	//  Fetch the id of the component of the templated type
	template<class T>
	inline ObjectID GetComponent() const {
		return components[T::GetTypeID()];
	}

	//  Get the list of child GameObjects
	std::vector<ObjectID>& GetChildren() {
		return children;
	}

	//  Get the list of child Components
	std::vector<ObjectID>& GetComponents() {
		return components;
	}

	//  Remove a child from either the Component or Child list (template specialization)
	template<class T>
	void DeassignChild(ObjectID id = 0) {
		int typeID = T::GetTypeID();
		if (typeID != GetTypeID() && typeID < components.size()) {
			components[typeID] = -1;
		} else {
			std::cout << typeid(T).name() << " cannot be removed as a component" << std::endl;
		}
	}

	//  Remove a child using RTTI IDs
	void DeassignChild(int typeID, ObjectID id) {
		if (typeID != GetTypeID() && typeID < components.size()) {
			components[typeID] = -1;
		} else if (typeID == GetTypeID()) {
			for (auto it = children.begin() ; it != children.end(); it++) {
				if ((*it) == id) {
					children.erase(it);
					return;
				}
			}			
		} else {
			std::cout << typeID << " cannot be removed as a component" << std::endl;
		}
	}



private:
	std::vector<ObjectID> components;
	std::vector<ObjectID> children;
};


//  A specialization of the AssignChild function specific to GameObjects as these are stored separately
template<>
inline void GameObject::AssignChild<GameObject>(ObjectID id) {
	children.push_back(id);
}

template<>
inline void GameObject::DeassignChild<GameObject>(ObjectID id) {
	int typeID = GetTypeID();
	if (typeID == GetTypeID() && typeID < children.size() && id != 0) {
		for (auto it = children.begin() ; it != children.end(); it++) {
			if ((*it) == id) {
				children.erase(it);
				return;
			}
		}
	} else {
		std::cout << typeid(GameObject).name() << " cannot be removed as a child" << std::endl;
	}
}
}