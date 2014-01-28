#pragma once
#include "LookupTable.h"
#include <vector>
#include "Component.h"
#include <iostream>

class ComponentManager
{
public:
	ComponentManager(){};
	~ComponentManager(){};

	template<class T>
	void AddComponentTypeToManager()
	{
		if (interfaces.size() <= T.GetComponentTypeID()) {
			interfaces.resize(T.GetComponentTypeID()+1);
		}
		interfaces[T.GetComponentTypeID()] = (LookupTableInterface*)&T.GetList();
	}

	void DeleteComponent(ComponentType id, ObjectID object) {
		if (interfaces.size() <= id) {
			std::cout << "Attempting to Delete Component of Unmanaged Type" << std::endl;
			return;
		}

		if (interfaces[id]->Exists(object)) {
			interfaces[id]->Remove(object);
		} else {
			std::cout << "Attempted to Delete Component That Does Not Exist" << std::endl;
		}
	}

	template<class T>
	void Delete(ObjectID object) {
		T::DeleteFromStorage(object);
	}

	template<class T>
	T& GetComponent(ObjectID id) {
		return T::Get(id);
	}

	template<class T>
	ObjectID NewComponent() {
		return T::New();
	}


private:
	std::vector<LookupTableInterface*> interfaces;
}
