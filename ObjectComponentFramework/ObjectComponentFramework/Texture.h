#pragma once
#include <D3D11.h>
#include "LookupTable.h"
#include <string>
#include <map>
#include <D3DX11tex.h>

typedef unsigned char TextureType;
#define TEXTUREDIR "Assets/Textures/"

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

	static T& Get(rabd::ObjectID id){return textureStorage.Get(id);}
	static TextureType GetTextureTypeID(){return textureTypeID;}
	static ID3D11ShaderResourceView* GetTexture(rabd::ObjectID id) {return Get(id).GetTexture();}
	static void DeleteFromStorage(rabd::ObjectID id) {textureStorage.Remove(id);}

	static rabd::ObjectID New(std::string file) {
		rabd::ObjectID newItem = T().CopyToStorage();
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

	rabd::ObjectID CopyToStorage(){
		lookup = textureStorage.Add(*(static_cast<T*>(this)));
		textureStorage.Get(lookup).SetLookup(lookup);
		return lookup;
	}

	bool Texture::Initialise()
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
	void SetLookup(rabd::ObjectID id) {lookup = id;}

	rabd::ObjectID lookup;
	std::string filename;
	ID3D11ShaderResourceView* texture;
private:
	static rabd::DynamicLookupTable<T> textureStorage;
	static std::map<std::string, ID3D11ShaderResourceView*> resourceLookup;
	static bool textureClassCreated;
	static TextureType textureTypeID;
};

template<class T> rabd::DynamicLookupTable<T> Texture<T>::textureStorage;
template<class T> std::map<std::string, ID3D11ShaderResourceView*> Texture<T>::resourceLookup;
template<class T> bool Texture<T>::textureClassCreated = false;
template<class T> TextureType Texture<T>::textureTypeID;