// Filename: graphicsclass.h
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>


#include "d3dclass.h"
#include "lightshaderclass.h"
#include "ShaderLibrary.h"
#include "LookupTable.h"
#include "AmbientNormalDeferredShader.h"
#include "RenderTarget.h"
#include "TextureToScreenShader.h"
#include "DeferredLightingShader.h"

class World;
class GraphicsClass
{
private:
	struct VertexType 
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(HWND hwnd, D3DClass* d3d);
	void Shutdown();

	bool Frame(World& world, bool deferred);
private:

	bool Render(World& world);
	bool RenderDeferred(World& world);

private:
	D3DClass* m_D3D;
	ShaderLibrary shaderLibrary;


	AmbientNormalDeferredShader m_deferred;
	TextureToScreenShader m_final;
	DeferredLightingShader m_lighting;
	RenderTarget renderTarget;
	RenderTarget lightingRenderTarget;

	////////////////////////////////////////////////////////////////
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif
