/*
	File:		Manager.h
	Version:	1.0
	Date:		26th February 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	Manager
	Requires:	LookupTable.h

	Description:
	
*/


#pragma once

#include "LookupTable.h"

#include <map>
#include <iostream>

namespace rabd
{
	class Manager
	{
	public:
		//  Default Constructor, set number of registered classes to zero for safety
		Manager() : registeredClasses(0)
		{
		}

		//  Default destructor, clean up the storage objects
		virtual ~Manager()
		{
			for (int i = 0; i < registeredClassLists.size(); ++i) {
				if (registeredClassLists[i] != nullptr) {
					delete registeredClassLists[i];
				}
			}
		}

		//  Create a stack for storing objects of the templated type
		//  Currently this requires the managed type to adhere to the RTTI model used
		//  by components.  This is for speed.
		template <class T>
		void RegisterType() {
			const int typeID = T::GetTypeID();
			if (registeredClassLists.size() <= typeID) {
				registeredClassLists.resize(typeID+1, nullptr);
			}
			if (registeredClassLists[typeID] == nullptr) {
				registeredClassLists[typeID] = new LookupTable<T>;
				registeredClasses++;
			}
		}

		//  Stop managing the templated type, deletes all objects of that type.
		template <class T>
		void DeregisterType() {
			const int typeID = T::GetTypeID();
			if (registeredClassLists.size <= typeID || registeredClassLists[typeID] == nullptr) {
				std::cout << typeid(T).name() << "Is an Unregistered type and could not be deregistered!" << std::endl;
			} else {
				delete registeredClassLists[typeID];
				registeredClassLists[typeID] = nullptr;
				registeredClasses--;
			}
		}

		//  Get the object of the specified type referenced by the passed ID
		template <class T>
		T& Get(ObjectID id) {
			const int typeID = T::GetTypeID();
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTable<T>* table = static_cast<LookupTable<T>*>(registeredClassLists[typeID]);
				return table->Get(id);
			} else {
				std::cout << typeid(T).name() << " is an unmanaged type, cannot get objects of this type" << std::endl;
				return T();
			}
		}

		//  An alternative Getter that uses the Component RTTI system to fetch objects
		//  more type agnostically as void pointers.  Avoid unless absolutely necessary.
		void* Get(int typeID, ObjectID id) {
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTableInterface* table = registeredClassLists[typeID];
				return table->GetTypeless(id);
			} else {
				std::cout << typeID << " is an unmanaged type, cannot get objects of this type" << std::endl;
				return NULL;
			}
		}

		//  Creates a new object of the templated type and hands back a reference ID.
		//  If the type is not curently being managed then it outputs an error and returns
		//  an invalid ID
		template <class T>
		ObjectID New() {
			const int typeID = T::GetTypeID();
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTable<T>* table = static_cast<LookupTable<T>*>(registeredClassLists[typeID]);
				return table->Push();
			} else {
				std::cout << typeid(T).name() << " is an unmanaged type, cannot create objects of this type" << std::endl;
				return -1;
			}			
		}

		//  Create a new object by copying a passed object.  Return an ID lookup to the new object.
		//  If the type is not currently being managed then it outputs an error and returns an
		//  invalid ID.
		template <class T>
		ObjectID New(T& object) {
			const int typeID = T::GetTypeID();
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTable<T>* table = static_cast<LookupTable<T>*>(registeredClassLists[typeID]);
				return table->Push(object);
			} else {
				std::cout << typeid(T).name() << " is an unmanaged type, cannot create objects of this type" << std::endl;
				return -1;
			}			
		}

		//  Moves an object into the storage structure using rValue references.  Returns an ID lookup to
		//  the object's new location.
		//  If the type of the object is not a managed type then it outputs an error and returns an
		//  invalid ID
		template <class T>
		ObjectID Move(T&& object) {
			const int typeID = T::GetTypeID();
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTable<T>* table = static_cast<LookupTable<T>*>(registeredClassLists[typeID]);
				return table->Push(object);
			} else {
				std::cout << typeid(T).name() << " is an unmanaged type, cannot create objects of this type" << std::endl;
				return -1;
			}			
		}

		//  Delete the object of the templated type referenced by passed ID
		//  If the templated type is not being managed then this function outputs an error
		template<class T>
		void Delete(ObjectID id) {
			const int typeID = T::GetTypeID();
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTableInterface* table = registeredClassLists[typeID];
				table->Remove(id);
			} else {
				std::cout << typeid(T).name() << " is an unmanaged type, cannot remove objects of this type" << std::endl;
			}		
		}

		//  Delete an object using RTTI.  Pass in fetched RTTI id and object ID
		//  If the templated type is not being managed then this function outputs an error
		void Delete(int typeID, ObjectID id) {
			if (typeID < registeredClassLists.size() && registeredClassLists[typeID] != nullptr) {
				LookupTableInterface* table = registeredClassLists[typeID];
				table->Remove(id);
			} else {
				std::cout << typeID << " is an unmanaged type, cannot remove objects of this type" << std::endl;
			}		
		}

		template<class T>
		bool IsManaged() const {
			return (registeredClassLists[T::GetTypeID()] != nullptr);
		}
	private:
		int registeredClasses;
		std::vector<rabd::LookupTableInterface*> registeredClassLists;
	protected:
	};
}