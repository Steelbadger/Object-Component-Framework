#pragma once
#include <map>

#include "ShaderInterface.h"

class ShaderLibrary
{
public:
	enum Shaders {NORMAL, DEFERRED};
	ShaderLibrary();
	~ShaderLibrary();

	void Initialise(ID3D11Device*, HWND);
	
	ShaderInterface* GetShader(Shaders s);

private:
	std::map<Shaders, ShaderInterface*> shaders;
};