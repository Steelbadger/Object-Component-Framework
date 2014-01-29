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
		D3DXVECTOR3 padding;
	};

	struct CameraBufferType
	{
//		D3DXMATRIX invProj;
		D3DXVECTOR4 topLeft;
		D3DXVECTOR4 topRight;
		D3DXVECTOR4 bottomLeft;
		D3DXVECTOR4 bottomRight;
	};
public:
	DeferredLightingShader();
	~DeferredLightingShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* dc, RenderTarget& renderTarget, ObjectID cameraObject, ObjectID light);

private:
	bool InitializeShader(ID3D11Device*, HWND, char*, char*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, char*);

	bool SetShaderParameters(ID3D11DeviceContext* dc, RenderTarget& renderTarget, ObjectID cameraObject, ObjectID light);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11SamplerState* m_sampleState;
};