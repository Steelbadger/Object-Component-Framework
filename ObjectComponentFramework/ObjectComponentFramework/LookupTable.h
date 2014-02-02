#pragma once

#include <vector>
#include <list>
#include <map>

typedef int ObjectID;

class LookupTableInterface {
public:
	LookupTableInterface(){;}
	virtual ~LookupTableInterface(){;}
	virtual void Remove(ObjectID object) = 0;
	virtual bool Exists(ObjectID object) const = 0;
	virtual int Size() const = 0;
	virtual int Capacity() const = 0;
	virtual int Count() const = 0;
};
 

template<class T>
class DynamicLookupTable : public LookupTableInterface
{
private: 
    std::vector<T> storage;
	std::list<ObjectID> spacesStack;
	std::map<ObjectID, T*> usedMap;

public:
    DynamicLookupTable() {
    }

	virtual ~DynamicLookupTable() {;}
    
    ObjectID Add(T& object){
		ObjectID outID;
		if (spacesStack.size() > 0) {
			outID = spacesStack.back();
			storage[outID] = std::move(object);
			spacesStack.pop_back();
		} else {
			outID = storage.size();
			storage.push_back(std::move(object));
		}
		usedMap[outID] = &storage[outID];
		return outID;
    }
    
    virtual void Remove(ObjectID object) {
		if (Exists(object)) {
			spacesStack.push_back(object);
			usedMap.erase(object);
		}
    }
    
    virtual bool Exists(ObjectID object) const {
		return (usedMap.count(object) > 0);
    }

	T& Get(ObjectID object) {
		if (Exists(object)) {
			return storage[object];
		} else {
			return T();
		}
	}

	virtual int Size() const {
		return storage.size();
	}

	virtual int Capacity() const {
		return storage.capacity();
	}

	virtual int Count() const {
		return usedMap.size();
	}
};