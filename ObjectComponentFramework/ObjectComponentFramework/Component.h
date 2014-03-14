/*
	File:		Component.h
	Version:	2.1
	Date:		26th February 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	Component
	Requires:	ComponentBase

	Description:
	Component ties into ComponentBase and allows for limited RTTI in all classes that inherit from it.
	Each new class is assigned an integer id that counts up from zero, this can then be used
	to store references to those types in a type-agnostic fashion.
*/



#pragma once
#include "ComponentBase.h"

namespace rabd
{
	class ObjectManager;

	template<class T>
	class Component : public ComponentBase
	{
	public:
		//  Default Constructor
		Component() : manager(nullptr), parentID(-1)
		{}
		//  Alternative Constructor
		Component(int parent) : parentID(parent)
		{}

		//  Default Destructor
		virtual ~Component(){}

		//  function to tell the component which GameObject is its parent
		inline void SetParentID(int id) {
			parentID = id;
		}

		inline bool HasParent() const {
			return (parentID >= 0);
		}

		//  getter for the parent ID (always a GameObject)
		inline int GetParentID() const {
			return parentID;
		}

		//  getter for the derived component type ID
		static int GetTypeID() {
			return uniqueID;
		}

		//  Give the component a pointer to the object managing it
		inline void SetManager(ObjectManager* m) {
			manager = m;
		}
		
	protected:
		ObjectManager* manager;
	private:
		static const int uniqueID;
		int parentID;
	};

	template<class T> const int Component<T>::uniqueID = GetUniqueID<T>();
}