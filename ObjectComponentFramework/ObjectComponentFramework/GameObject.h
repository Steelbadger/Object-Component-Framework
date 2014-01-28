#pragma once
#include "LookupTable.h"
#include "Component.h"
#include <vector>
#include <map>
#include "UtilityFunctions.h"


//  A GameObject is both a component and an 'owner' of components
//  A GameObject may act like a component (with a parent)
//  Or it may act like a root node in the hierarchy with no parent
class GameObject : public Component<GameObject>
{
public:
	GameObject();
	~GameObject();

	//  Return the lookup for a component of type specified by the unique id of
	//  the component type of interest.  Largely ignored now
	ObjectID GetComponentOfType(ComponentType type);

	//  Make a pre-existing GameObject object a child of this object
	//  This could be imporoved by simply making it a template specialisation
	//  of the AddComponent functions
	void AddChild(ObjectID id);

	//  Unparent the component of type specified by it's unique id
	//  (Does not delete the component from the component storage, JUST unparents)
	//  Largely ignored now
	void RemoveComponent(ComponentType type);

	//  Unparent a prexisting GameObject from this GameObject
	void RemoveChild(ObjectID id);

	//  Return the list of all GameObjects that are children of this GameObject
	std::vector<ObjectID> GetChildren();

	//  Make a pre-existing component a child of this GameObject
	//  uses the Component type unique ID to specify component type
	//  Largely ignored now in favour of templated functions
	void AddComponent(ObjectID id, ComponentType type);

	//  This is the primary method of adding a component to a GameObject
	//  It creates a new component of the type T and adds it to the gameObject
	//  It then returns the ID of that component in the component lookup table
	//  incase the user wishes to modify that component
	template<class T> ObjectID AddComponent() {
		ComponentType type = T::GetComponentTypeID();		
		ObjectID newComp = T::New(GetID());
		components[type] = newComp;
		return newComp;
	}

	template<class T> bool HasComponent() {
		ComponentType type = T::GetComponentTypeID();
		if (components.count(type) != 0) {
			return true;
		} else {
			return false;
		}
	}

	//  This removes the Component of the type T from the GameObject
	//  It COMPLETELY DELETES that component.
	template<class T> void RemoveComponent() {
		ComponentType type = T::GetComponentTypeID();
		ObjectID comp = 0;
		if (components.count(type) != 0) {
			comp = components[type];
			components.erase(type);
			T::DeleteFromStorage(comp);
		} else {
			Error("Cannot Delete, No Component of this Type");
			//panic
		}
	}

	//  Retrieve the component of the type T from the GameObject as a reference
	//  NOTE:  if the Component does not exist then it CREATES ONE.
	template<class T> T& GetComponent(){
		ComponentType type = T::GetComponentTypeID();
		if(components.count(type) != 0) {
			ObjectID compID = components[type];
			return T::Get(compID);
		} else {
			//Warning("No Component of type, creating new Component in place");
			ObjectID compID = AddComponent<T>();
			return T::Get(compID);
		}
	}

	//  Get the ID of the component of type T
	//  NOTE:  If the Component of that type does not exist then it CREATES ONE.
	template<class T> ObjectID GetComponentReference() {
		ComponentType type = T::GetComponentTypeID();
		ObjectID compID;
		if(components.count(type) != 0) {
			compID = components[type];
		} else {
			Warning("No Component of type, creating new Component in place and passing reference");
			compID = AddComponent<T>();
		}
		return compID;
	}


	//  Static access to the templated GetComponent function
	//  This is just for shortening/simplifying the code necessary to fetch a component
	template<class T> static T& GetComponent(ObjectID id) {
		return GameObject::Get(id).GetComponent<T>();
	}

	//  Static access to the templated GetComponentReference function
	//  This is just for shortening/simplifying the code necessary to fetch a component reference
	template<class T> static ObjectID GetComponentReference(ObjectID id) {
		return GameObject::Get(id).GetComponentReference<T>();
	}

	//  Add a component of type T to the GameObject id
	//  This is just for shortening/simplifying the code necessary to add a new component
	template<class T> static ObjectID AddComponent(ObjectID id){
		return GameObject::Get(id).AddComponent<T>();
	}

	template<class T> static bool HasComponent(ObjectID id){
		return GameObject::Get(id).HasComponent<T>();
	}

	static void SetParentChild(ObjectID parent, ObjectID child);

private:
	std::map<ComponentType, ObjectID> components;	//  map because lookup is simple unsigned char type, may be sparse
	std::vector<ObjectID> childGameObjects;		//  array lookup means nothing, just storage
	bool modified;
};