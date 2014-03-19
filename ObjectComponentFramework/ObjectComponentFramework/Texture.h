#pragma once
#include <D3D11.h>
#include <string>
#include <map>
#include <D3DX11tex.h>

typedef signed int TextureType;
#define TEXTUREDIR "Assets/Textures/"

#include "LookupTable.h"
#include "TextureBase.h"

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
			return manager->Get<T>(id);
		}

		static TextureType GetTextureTypeID() {
			return uniqueID;
		}

		static ID3D11ShaderResourceView* GetTexture(ObjectID id) {
			return Get(id).GetTexture();
		}

		//  getter for the derived component type ID
		static int GetTypeID() {
			return uniqueID;
		}

		static rabd::ObjectID New(std::string file) {
			rabd::ObjectID newItem = manager->New<T>(T());
			manager->Get<T>(newItem).SetFile(file);
			manager->Get<T>(newItem).Initialise();
			return newItem;
		}

		static void DeleteFromStorage(ObjectID id) {
			manager->Delete<T>(id);
		}

	protected:
		//  Default Constructor, hidden as all textures should be managed
		Texture()
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
		static std::map<std::string, ID3D11ShaderResourceView* > resourceLookup;
	};

	template<class T> const int Texture<T>::uniqueID = GetUniqueID<T>();
	template<class T> std::map<std::string, ID3D11ShaderResourceView* > Texture<T>::resourceLookup;
}