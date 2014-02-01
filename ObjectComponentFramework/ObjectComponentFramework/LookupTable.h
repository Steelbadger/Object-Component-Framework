#pragma once

#include <vector>

typedef int ObjectID;

class LookupTableInterface {
public:
	LookupTableInterface(){;}
	virtual ~LookupTableInterface(){;}
	virtual void Remove(ObjectID object) = 0;
	virtual bool Exists(ObjectID object) = 0;
	virtual int Size() = 0;
};
 

template<class T, int InitialSize = 1>
class DynamicLookupTable : public LookupTableInterface {
private:
	struct Storage {
        Storage():used(false){}
        Storage(T& data):element(data), used(true){}
        T element;
        bool used;        
    };


public:
    DynamicLookupTable() {
		storage.reserve(1000);
		spacesStack.reserve(1000);
        for (int i = 0; i < InitialSize; i++) {
			spacesStack.push_back((InitialSize-1)-i);
			storage.push_back(Storage());
        }
    }

	virtual ~DynamicLookupTable() {;}
    
    ObjectID Add(T& object){
		ObjectID outID;
		if (spacesStack.size() > 0) {
			outID = spacesStack.back();
			storage[outID] = Storage(object);
			spacesStack.pop_back();
		} else {
			outID = storage.size();
			storage.push_back(Storage(object));
		}
		return outID;
    }
    
    virtual void Remove(ObjectID object) {
		if (Exists(object)) {
			spacesStack.push_back(object);
			storage[object].used = false;
		}
    }
    
    virtual bool Exists(ObjectID object){
		if (object >= 0 && object < storage.size()) {
			return storage[object].used;
		} else {
			return false;
		}
    }

	T& Get(ObjectID object) {
		if (Exists(object)) {
			return storage[object].element;
		} else {
			return T();
		}
	}

	virtual int Size() {
		return storage.size();
	}

private:
    std::vector<Storage> storage;
    std::vector<ObjectID> spacesStack; 
};