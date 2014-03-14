/*
	File:		LookupTable.h
	Version:	3.1
	Date:		25th February 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	LookupTable, LookupTableInterface
	Requires:	Nothing

	Description:
	The lookup table is a custom storage class that combines a linked list and vector.

	The purpose of the class is to take advantage of principle of locality when storing objects,
	wile keeping much of the ease of use of an std::map.  When objects are pushed into this structure 
	they are placed in the most recently vacated space.  The basic storage structure is an std::vector
	allowing for the container to have a dynamic size.

	The type stored in the class contains placeholder bytes into which objects can be allocated
	this ensures that the lifetime of the stored objects is exactly as long as requested.

	This structure should only be used to store objects with a size greater than zero (they must have at least one member
	variable).  This is reasonable as the structure is designed to leverage principle of locality, if there
	is no member data then locality is irrelevant.
*/

#pragma once


//  C++ standard library includes
#include <vector>
#include <list>
#include <map>


namespace rabd {

//  Typedef identifying the lookup IDs used in the table.
//  This is an int to allow for bounds checking.
typedef int ObjectID;


class LookupTableInterface {
public:
	//  Empty constructor
	LookupTableInterface(){}

	//  Empty virtual destructor
	virtual ~LookupTableInterface(){}

	//  Hook for the Remove function signature, allows for removal of elements
	virtual void Remove(ObjectID object) = 0;

	//  Hook for the Exist function signature, allows for checking of state of elements
	virtual bool Exists(ObjectID object) const = 0;

	//  Hook for fetching the size of the structure
	virtual int Size() const = 0;

	//  Hook for fetching the capacity of the structure (from std::vector::capacity())
	virtual int Capacity() const = 0;

	//  Hook for retrieving the number of items stored in the structure
	virtual int Count() const = 0;

	//  Hook for retrieving objects from the structure in a type agnostic way
	virtual void* GetTypeless(ObjectID object) const = 0;
};	//  End LookupTableInterface declaration

template<class T>
class LookupTable : public LookupTableInterface
{
private:
	//  The byte-size of the type to be stored
	static const int elemSize = sizeof(T);

	//  The storage type to be held in the vector
	class Storage {
	public:
		//  Allocate enough memory to hold the stored type
		char data[elemSize];

		//  Track the state of the storage node
		bool used;

		//  Use a linked list to traverse the nodes
		ObjectID up;
		ObjectID down;
	};	//  End Storage class declaration

public:
	//  The iterator type used for standard library style storage traversal
	class Iterator {
	public:
		//  Give the parent lookup table class access to private members
		//  This avoids exposing getters and setters to the user.
		friend class LookupTable<T>;

		//  Default constructor does nothing
		Iterator(){};

		//  Default destructor does nothing
		~Iterator(){};

		//  Overload the dereference operator to access the object at the iterator
		T& operator*(){
			return target->Get(id); 
		}

		//  Overload the dereference operator to access the object at the iterator
		const T& operator*() const {
			return target->Get(id); 
		}

		//  Overload the pointer member operator to access the members of the stored data
		T* operator->() {
			return &target->Get(id);
		}

		//  Overload the pointer member operator to access the members of the stored data
		const T* operator->() const {
			return &target->Get(id);
		}

		//  Overload the increment operator to traverse the linked list forwards
		Iterator& operator++(){
			id = target->storage[id].up;
			return *this;
		}

		//  Overload the decrement operator to traverse the linked list backwards
		Iterator& operator--(){
			id = target->storage[id].down;
			return *this;
		}

		//  Overload the increment operator to traverse the linked list forwards
		Iterator& operator++(int){
			id = target->storage[id].up;
			return *this;
		}

		//  Overload the decrement operator to traverse the linked list backwards
		Iterator& operator--(int){
			id = target->storage[id].down;
			return *this;
		}



		//  Overload the boolean equality operator to check similarity of members
		bool operator==(const Iterator& rhs) const {
			return ((id == rhs.id) && (target == rhs.target));
		}

		//  Overload the boolean inequality operator to check for dissimilarity of members
		bool operator!=(const Iterator& rhs) const {
			return ((id != rhs.id) || (target != rhs.target));
		}
	private:
		//  The array lookup for the iterator
		ObjectID id;

		//  The table hosting the iterator
		LookupTable<T>* target;
	};	//  End Iterator declaration

	//  Constructor, set head, tail and size
	LookupTable():
		head(-1), tail(-1), numElements(0)
	{}

	//  Destruct, iterate through all items and call their destructors
	~LookupTable()
	{
		for (unsigned int i = 0; i < storage.size(); i++) {
			if (storage[i].used == true) {
				Remove(i);
			}
		}
	}

	//  Remove and destruct the object at array position 'object'.
	//  Handle any error cases without a crash, display a warning message
	void Remove(ObjectID object)
	{
		//  The only time the tail is a non-legal array lookup is when the structure is empty
		if (tail == -1) {
			std::cout << "Structure is empty, cannot delete anything" << std::endl;
			return;
		}
		//  Make sure the entered ID is within the array bounds
		if (static_cast<unsigned int>(object) >= storage.size() || object < 0) {
			std::cout << "Object ID " << object << " is outside structure bounds, cannot delete element" << std::endl;
			return;
		}
		//  Make sure the entered ID isn't attempting to delete an already deleted object
		if (storage[object].used == false) {
			std::cout << "Object ID " << object << " is not in use, cannot delete element" << std::endl;
			return;
		}

		//  If removing the first item in the list then move the tail to the next item
		if (object == tail) {
			tail = storage[object].up;
			//  If tail is equal to head then the array has been made empty, set tail to -1 to 
			//  make this obvious
			if (tail == head) {
				tail = -1;
			}
		//  Relink the list around the deleted element
		} else {
			storage[storage[object].down].up = storage[object].up;
		}
		if (storage[head].down != object) {
			storage[storage[head].down].up = object;
		}
		storage[storage[object].up].down = storage[object].down;
		storage[object].up = head;
		storage[object].down = storage[head].down;
		storage[head].down = object;

		//  Note the object as deleted
		storage[object].used = false;
		//  Call the object destructor
		((T*)storage[object].data)->~T();
		//  New items should go in the newly vacated space to improve locality.
		head = object;
		//  Keep a count of the number of elements
		numElements--;
	}

	//  Check if the given ID is a valid object
	bool Exists(ObjectID object) const
	{
		return (static_cast<unsigned int>(object) < storage.size() && object >= 0 ? storage[object].used : false);
	}

	//  Find the size of the underlying vector
	int Size() const
	{
		return storage.size();
	}

	//  Find the capacity of the underlying vector
	int Capacity() const
	{
		return storage.capacity();
	}

	//  Return the number of elements stored in the structure
	int Count() const
	{
		return numElements;
	}

	//  Return the object referenced by ID type agnostically
	void* GetTypeless(ObjectID object) const
	{
		return (void*)&storage[object].data;
	}

	//  Get the item at id by reference
	T& Get(ObjectID id)
	{
		return *reinterpret_cast<T*>(&storage[id].data[0]);
	}

	//  Copy an item by reference into the storage structure, return the id to retrieve that object
	ObjectID Push(T& object)
	{
		//  If head is -1 then the vector has no memory allocated
		//  create a new element to work with
		if (head == -1) {
			storage.push_back(Storage());
			ObjectID newHead = storage.size()-1;
			storage[newHead].down = -1;
			storage[newHead].up = -1;
			head = newHead;
		}

		//  The new object will be placed at the current head
		ObjectID out = head;

		//  If filling the head with data would cause the vector to be full then
		//  create a new empty head node to keep iterators working
		if (storage[head].up == -1) {
			AddEmptyHead();
		}

		//  Interpret the reserved space as a pointer of the stored type and construct an object in that position
		T* temp = reinterpret_cast<T*>(storage[out].data);
		new(temp) T(object);
		numElements++;
		storage[out].used = true;

		//  If the tail was -1 (meaning the vector was previously empty) make the new object the new tail
		if (tail == -1) {
			tail = out;
		}

		//  Huzzah
		return out;
	}

	//  Copy an item by reference into the storage structure, return the id to retrieve that object
	ObjectID Push(T&& object)
	{
		//  If head is -1 then the vector has no memory allocated
		//  create a new element to work with
		if (head == -1) {
			storage.push_back(Storage());
			ObjectID newHead = storage.size()-1;
			storage[newHead].down = -1;
			storage[newHead].up = -1;
			head = newHead;
		}

		//  The new object will be placed at the current head
		ObjectID out = head;

		//  If filling the head with data would cause the vector to be full then
		//  create a new empty head node to keep iterators working
		if (storage[head].up == -1) {
			AddEmptyHead();
		}

		//  Interpret the reserved space as a pointer of the stored type and construct an object in that position
		T* temp = reinterpret_cast<T*>(storage[out].data);
		new(temp) T(object);
		numElements++;
		storage[out].used = true;

		//  If the tail was -1 (meaning the vector was previously empty) make the new object the new tail
		if (tail == -1) {
			tail = out;
		}

		//  Huzzah
		return out;
	}

	//  Copy an item by reference into the storage structure, return the id to retrieve that object
	ObjectID Push()
	{
		//  If head is -1 then the vector has no memory allocated
		//  create a new element to work with
		if (head == -1) {
			storage.push_back(Storage());
			ObjectID newHead = storage.size()-1;
			storage[newHead].down = -1;
			storage[newHead].up = -1;
			head = newHead;
		}

		//  The new object will be placed at the current head
		ObjectID out = head;

		//  If filling the head with data would cause the vector to be full then
		//  create a new empty head node to keep iterators working
		if (storage[head].up == -1) {
			AddEmptyHead();
		}

		//  Interpret the reserved space as a pointer of the stored type and construct an object in that position
		T* temp = reinterpret_cast<T*>(storage[out].data);
		new(temp) T();
		numElements++;
		storage[out].used = true;

		//  If the tail was -1 (meaning the vector was previously empty) make the new object the new tail
		if (tail == -1) {
			tail = out;
		}

		//  Huzzah
		return out;
	}

	//  Return an iterator to the first element in the linked list
	Iterator Begin()
	{
		Iterator it;
		it.id = tail;
		it.target = this;
		return it;
	}

	//  Return an iterator to the (empty) head element
	Iterator End()
	{
		Iterator it;
		it.id = head;
		it.target = this;
		return it;
	}

private:
	//  Helper function for adding a new empty head element to the end of a filled vector
	void AddEmptyHead() {
		storage.push_back(Storage());
		ObjectID newHead = storage.size()-1;
		storage[head].up = newHead;	
		storage[newHead].down = head;
		storage[newHead].up = -1;
		head = newHead;
	}

private:
	std::vector<Storage> storage;
	ObjectID head;
	ObjectID tail;
	int numElements;
};	//  End LookupTable declaration


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

	//  Return the object referenced by ID type agnostically
	void* GetTypeless(ObjectID object) const
	{
		return (void*)&storage[object];
	}
};
}	//  End rabd namespace
