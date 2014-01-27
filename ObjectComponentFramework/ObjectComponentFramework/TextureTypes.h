#pragma once
#include "Texture.h"

//  Define some basic texture types, most are vanilla, though Displacement Maps require
//  a value to specify the magnitude of the maximum displacement in the map.

//  Other possible textures might be 'Parallax Map' or 'Lightmap'
//  Also, reflection map could feasably be created here


//  The basic, unlit, colour map
class AmbientTexture : public Texture<AmbientTexture>
{
public:
	AmbientTexture(){};
	~AmbientTexture(){};
};

//  the 'shininess'
class SpecularMap : public Texture<SpecularMap>
{
public:
	SpecularMap(){};
	~SpecularMap(){};

	void SetShininessScale(float shin){shininessMax = shin;}
	float GetShininessScale(){return shininessMax;}
private:
	float shininessMax;
};

//  For altering the light normals across a single poly
//  to create illusion of high res geometry
class NormalMap : public Texture<NormalMap>
{
public:
	NormalMap(){};
	~NormalMap(){};
};

//  For use with tessellation, kind of like normal mapping
//  except this uses tessellation to CREATE high res geometry then
//  uses the displacement map to actually make the high res
//  model the right shape
class DisplacementMap : public Texture<DisplacementMap>
{
public:
	DisplacementMap(){};
	~DisplacementMap(){};
	void SetDisplacementMagnitude(float mag){displacementMagnitude = mag;}
	float GetDisplacementMagnitude(){return displacementMagnitude;}
private:
	float displacementMagnitude; 
};
