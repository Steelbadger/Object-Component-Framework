#pragma once
#include <D3D11.h>
#include "LookupTable.h"
#include <string>
#include <map>
#include <D3DX11tex.h>

typedef unsigned char TextureType;

class TextureBase
{
public:
	TextureBase(){}
	~TextureBase(){}

	static TextureType GetNumberOfTextureTypes(){return textureType;}
	static void SetDevice(ID3D11Device* dev){device = dev;}
protected:
	static TextureType textureType;
	static ID3D11Device* device;
};


template<class T>
class Texture : protected TextureBase
{
public:
	~Texture(){;}

	void SetFile(std::string file) {filename = file;}
	ID3D11ShaderResourceView* GetTexture() {return texture;}
	void DeleteFromStorage(){textureStorage.Remove(lookup);}

	static T& Get(ObjectID id){return textureStorage.Get(id);}
	static TextureType GetTextureTypeID(){return textureTypeID;}
	static ID3D11ShaderResourceView* GetTexture(ObjectID id) {return Get(id).GetTexture();}
	static void DeleteFromStorage(ObjectID id) {textureStorage.Remove(id);}

	static ObjectID New(std::string file) {
		ObjectID newItem = T().CopyToStorage();
		Get(newItem).SetFile(file);
		Get(newItem).Initialise();
		return newItem;
	}

protected:
	Texture(){
		if (textureClassCreated == false) {
			textureTypeID = textureType++;	
			textureClassCreated = true;
		}
	}

	ObjectID CopyToStorage(){
		lookup = textureStorage.Add(*(static_cast<T*>(this)));
		textureStorage.Get(lookup).SetLookup(lookup);
		return lookup;
	}

	bool Texture::Initialise()
	{
		if (resourceLookup.count(filename) == 0) {
			HRESULT result;
			// Load the texture in.
			result = D3DX11CreateShaderResourceViewFromFile(device, filename.c_str(), NULL, NULL, &texture, NULL);
			if(FAILED(result))
			{
				return false;
			}
			resourceLookup[filename] = texture;
		} else {
			texture = resourceLookup[filename];
		}
		return true;
	}
private:
	void SetLookup(ObjectID id) {lookup = id;}

	ObjectID lookup;
	std::string filename;
	ID3D11ShaderResourceView* texture;
private:
	static DynamicLookupTable<T> textureStorage;
	static std::map<std::string, ID3D11ShaderResourceView*> resourceLookup;
	static bool textureClassCreated;
	static TextureType textureTypeID;
};

template<class T> DynamicLookupTable<T> Texture<T>::textureStorage;
template<class T> std::map<std::string, ID3D11ShaderResourceView*> Texture<T>::resourceLookup;
template<class T> bool Texture<T>::textureClassCreated = false;
template<class T> TextureType Texture<T>::textureTypeID;