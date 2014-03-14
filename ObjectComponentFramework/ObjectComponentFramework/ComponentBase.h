/*
	File:		ComponentBase.h
	Version:	2.1
	Date:		26th February 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	ComponentBase
	Requires:	Nothing

	Description:
	ComponentBase allows for limited RTTI in all classes that inherit from it.
	Each new class is assigned an integer id that counts up from zero, this can then be used
	to store references to those types in a type-agnostic fashion.
*/

#pragma once


namespace rabd
{
	class ComponentBase
	{
	public:
		ComponentBase(){}
		virtual ~ComponentBase(){}

		static int GetNumberOfComponents() {
			return idCounter;
		}

		virtual void SetParentID(int parent) = 0;

	protected:
		template<class T>
		static int GetUniqueID() {
			static const int id(GetNextID());
			return id;
		}
	private:
		static inline int GetNextID() {
			return idCounter++;
		}
		static int idCounter;
	};

}