#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "RenderTarget.h"
#include <vector>

class DeferredLightingShader
{
private:
	struct LightBufferType
	{
		D3DXVECTOR4 lightColor;
		D3DXVECTOR4 lightDirection;
		float specularPower; 
		D3DXVECTOR3 topRight;
	};
public:
	DeferredLightingShader();
	~DeferredLightingShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* dc, RenderTarget& renderTarget, rabd::ObjectID cameraObject, rabd::ObjectID light, rabd::ObjectManager* manager);

private:
	bool InitializeShader(ID3D11Device*, HWND, const char*, const char*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const char*);

	bool SetShaderParameters(ID3D11DeviceContext* dc, RenderTarget& renderTarget, rabd::ObjectID cameraObject, rabd::ObjectID light, rabd::ObjectManager* manager);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11SamplerState* m_sampleState;
};