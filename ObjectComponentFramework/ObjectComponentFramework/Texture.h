#pragma once
#include <D3D11.h>
#include <string>
#include <map>
#include <D3DX11tex.h>

typedef signed int TextureType;
#define TEXTUREDIR "Assets/Textures/"

#include "LookupTable.h"
#include "TextureBase.h"
#include "Manager.h"

namespace rabd
{
	template<class T>
	class Texture : public TextureBase
	{
	public:
		//  Default Destructor
		virtual ~Texture(){}

		void SetFile(std::string file) {
			filename = file;
		}

		ID3D11ShaderResourceView* GetTexture() const {
			return texture;
		}

		static T& Get(ObjectID id) {
			return manager->Get(id);
		}

		static TextureType GetTextureTypeID() const {
			return uniqueID;
		}

		static ID3D11ShaderResourceView* GetTexture(ObjectID id) const {
			return Get(id).GetTexture();
		}

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
		static int GetTypeID() const {
			return uniqueID;
		}

		//  Give the component a pointer to the object managing it
		inline static void SetManager(Manager* m) {
			manager = m;
		}

		static rabd::ObjectID New(std::string file) {
			rabd::ObjectID newItem = manager->New<T>(T());
			manager->Get(newItem).SetFile(file);
			manager->Get(newItem).Initialise();
			return newItem;
		}

		static void DeleteFromStorage(ObjectID id) {
			manager->Delete<T>(id);
		}

	protected:
		static Manager* manager;

		//  Default Constructor, hidden as all textures should be managed
		Texture() : manager(nullptr), parentID(-1)
		{}

		bool Initialise()
		{
			if (resourceLookup.count(filename) == 0) {
				HRESULT result;
				// Load the texture in.
				std::string longName = TEXTUREDIR + filename;
				result = D3DX11CreateShaderResourceViewFromFile(device, longName.c_str(), NULL, NULL, &texture, NULL);
				if(FAILED(result))
				{
					std::cout << "Texture: " << filename << "Failed!" << std::endl;
					return false;
				}
				resourceLookup[filename] = texture;
			} else {
				texture = resourceLookup[filename];
			}
			return true;
		}

	private:
		static const int uniqueID;

		std::string filename;
		ID3D11ShaderResourceView* texture;
		static std::map<signed int, ID3D11ShaderResourceView* > resourceLookup;
	};

	template<class T> const int Texture<T>::uniqueID = GetUniqueID<T>();
	template<class T> std::map<signed int, ID3D11ShaderResourceView* > Texture<T>::resourceLookup;
	template<class T> Manager* Texture<T>::manager;
}