#include "ShaderLibrary.h"

//#include "lightshaderclass.h"
//#include "NormalMapShaderClass.h"
//#include "AmbientNormalDeferredShader.h"



ShaderLibrary::ShaderLibrary()
{

}


ShaderLibrary::~ShaderLibrary()
{

}


void ShaderLibrary::Initialise(ID3D11Device* dev, HWND handle)
{
	//shaders[LIT] = new LightShaderClass;
	//shaders[LIT]->Initialize(dev, handle);

	//shaders[NORMAL] = new NormalMapShaderClass;
	//shaders[NORMAL]->Initialize(dev, handle);

	//shaders[DEFERRED] = new AmbientNormalDeferredShader;
	//shaders[DEFERRED]->Initialize(dev, handle);
}


ShaderInterface* ShaderLibrary::GetShader(Shaders s)
{
	if (shaders.count(s) > 0) {
		return shaders[s];
	} else {
		return nullptr;
	}
}