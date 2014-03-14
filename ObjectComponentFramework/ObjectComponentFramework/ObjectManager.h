/*
	File:		ObjectManager.h
	Version:	1.0
	Date:		26th February 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	ObjectManager
	Requires:	Manager.h, GameObject.h

	Description:
	This class is a specialisation of the Manager class, aimed at managing an object-component system and providing
	useful functions to make this less onerous
*/


#pragma once

#include "Manager.h"
#include "GameObject.h"

namespace rabd
{
	class ObjectManager : public Manager
	{
	public:
		//  Constructor is default
		ObjectManager(){};
		//  Destructor is default
		~ObjectManager(){};

		//  Create a new GameObject (interfaces with templated function from superclass)
		ObjectID NewObject() {
			return New<GameObject>();
		}

		//  Destruct an object and ALL of its components
		void DeleteObjectAndComponents(ObjectID object) {
			std::vector<ObjectID> components = Get(object).GetComponents();
			for (int i = 0; i < components.size(); ++i) {
				if (components[i] >= 0)
					Delete(i, components[i]);
			}
			Delete<GameObject>(object);
		}

		//  A bunch of functions for creating an object AND all its components with a single function
		template<class T>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T>(out);
			return out;
		}

		template<class T1, class T2>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			return out;
		}

		template<class T1, class T2, class T3>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4, class T5>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			CreateComponent<T5>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4, class T5, class T6>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			CreateComponent<T5>(out);
			CreateComponent<T6>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			CreateComponent<T5>(out);
			CreateComponent<T6>(out);
			CreateComponent<T7>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			CreateComponent<T5>(out);
			CreateComponent<T6>(out);
			CreateComponent<T7>(out);
			CreateComponent<T8>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			CreateComponent<T5>(out);
			CreateComponent<T6>(out);
			CreateComponent<T7>(out);
			CreateComponent<T8>(out);
			CreateComponent<T9>(out);
			return out;
		}

		template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		ObjectID CreateObjectAndComponents() {
			ObjectID out = New<GameObject>();
			CreateComponent<T1>(out);
			CreateComponent<T2>(out);
			CreateComponent<T3>(out);
			CreateComponent<T4>(out);
			CreateComponent<T5>(out);
			CreateComponent<T6>(out);
			CreateComponent<T7>(out);
			CreateComponent<T8>(out);
			CreateComponent<T9>(out);
			CreateComponent<T10>(out);
			return out;
		}

		//  Create a new object of the templated type and make it a child of the GameObject referenced by
		//  the passed ID.  Returns the ID of the new component.
		template<class T>
		ObjectID CreateComponent(ObjectID parent) {
			T temp;
			temp.SetParentID(parent);
			ObjectID out = New<T>(temp);
			Manager::Get<T>(out).SetManager(this);
			Get(parent).AssignChild<T>(out);
			return out;
		}

		template<class T>
		inline bool HasComponent(ObjectID id) {
			return Get(id).HasComponent<T>();
		}

		//  Get the component of type T attached to the GameObject referenced by the passed ID
		template<class T>
		T& GetComponent(ObjectID parent) {
			return Manager::Get<T>(Get(parent).GetComponent<T>());
		}

		GameObject& Get(ObjectID object) {
			return Manager::Get<GameObject>(object);
		}

		//  Create a parent-child relationship between an object of type T (child) and a GameObject (parent)
		template<class T>
		void SetParent(ObjectID child, ObjectID parent) {
			Manager::Get<T>(child).SetParentID(parent);
			Get(parent).AssignChild<T>(child);
		}

		//  Remove the parent-child relationship between an object of Type T (child) and a GameObject (parent)
		template<class T>
		void UnParent(ObjectID child, ObjectID parent) {
			Get<T>(child).SetParentID(-1);
			Get(parent).DeassignChild<T>(child);
		}

		//  A non templatised UnParent function can be used to Unparent objects using RTTI
		void UnParent(int childTypeID, ObjectID child, ObjectID parent) {
			ComponentBase* temp = static_cast<ComponentBase*>(Manager::Get(childTypeID, child));
			temp->SetParentID(-1);
			Get(parent).DeassignChild(childTypeID, child);
		}

	private:
	};

}