#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include "GameObject.h"

#include "World.h"

#define SHADERDIR "Assets/Shaders/"

class ShaderInterface
{
public:
	ShaderInterface(){};
	virtual ~ShaderInterface(){};
	
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(ID3D11DeviceContext* dc, rabd::ObjectID drawObject, World& world) = 0;
};
