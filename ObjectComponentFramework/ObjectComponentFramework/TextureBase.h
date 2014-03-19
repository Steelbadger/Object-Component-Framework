/*
	File:		ComponentBase.h
	Version:	1.0
	Date:		18th March 2014
	Authors:	Ross Davies

	Namespace:	rabd
	Exposes:	TextureBase
	Requires:	Nothing

	Description:
	TextureBase allows for limited RTTI in all classes that inherit from it.
	Each new class is assigned an integer id that counts up from zero, this can then be used
	to store references to those types in a type-agnostic fashion.
*/

#pragma once
#include <D3D11.h>
#include "Manager.h"

namespace rabd
{
	class TextureBase
	{
	public:
		TextureBase(){}
		virtual ~TextureBase(){}

		static int GetNumberOfTextureTypes() {
			return idCounter;
		}

		static void SetDevice(ID3D11Device* dev) {
			device = dev;
		}

		//  Give the component a pointer to the object managing it
		inline static void SetManager(Manager* m) {
			manager = m;
		}

	protected:
		template<class T>
		static int GetUniqueID() {
			static const int id(GetNextID());
			return id;
		}

		static ID3D11Device* device;
		static Manager* manager;
	private:
		static inline int GetNextID() {
			return idCounter++;
		}
		static int idCounter;
	};
}