#pragma once

#include "Texture.h"
#include "Component.h"
#include "ShaderInterface.h"
#include "ShaderLibrary.h"
#include <map>

//  A Material contains a collection of Textures and can retrieve 
//  textures by type (ie ambient, specular etc).  Conceptually similar
//  To the GameObject class, but stores Textures, not other components
//  This is a component that can be added to a GameObject

class Material : public Component<Material>
{
public:
	Material(){};
	~Material(){};



	//  Add a texture of Type T to the material, create it in place
	template<class T> void AddTexture(std::string file) {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = T::New(file);
		if (textures.count(type) == 0) {
			textures[type] = id;
		} else {
			//panic
		}
	}

	//  Remove the texture of type T from the material
	//  DELETE the texture
	template<class T> void RemoveTexture() {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = 0;
		if (textures.count(type) != 0) {
			id = textures[type];
			textures.erase(type);
			T::DeleteFromStorage(id);
		} else {
			//panic
		}			
	}

	//  Retrieve the directx texture resource associated with the texture of type T
	template<class T> ID3D11ShaderResourceView* GetTextureResource() {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = textures[type];
		return T::GetTexture(id);
	}

	//  Get the attached Texture of type T
	template<class T> T& GetTexture() {
		TextureType type = T::GetTextureTypeID();
		ObjectID id = textures[type];
		return T::Get(id);
	}

	ShaderLibrary::Shaders GetShader(){return shader;}
	void SetShader(ShaderLibrary::Shaders s){shader = s;}
private:
	std::map<TextureType, ObjectID> textures;		//  Store the texture IDs
	ShaderLibrary::Shaders shader;
};
