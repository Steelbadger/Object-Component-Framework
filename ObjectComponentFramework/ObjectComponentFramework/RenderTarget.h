#pragma once

#include <d3d11.h>
#include <vector>
#include <initializer_list>

class RenderTarget
{
public:
	enum Type {COLOUR, NORMAL, DEPTH, FLOAT_RGBA, FLOAT_R, HALFFLOAT_RGBA};

	RenderTarget();
	~RenderTarget();

	bool Initialize(ID3D11Device*, int width, int height, int numTargets);
	bool InitializeTEST(ID3D11Device*, int width, int height, int number, ...);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	std::vector<ID3D11ShaderResourceView*> GetShaderResourceViews();

private:
	std::vector<ID3D11Texture2D*> m_targetTextures;
	std::vector<ID3D11RenderTargetView*> m_targetViews;
	std::vector<ID3D11ShaderResourceView*> m_resourceViews;

	ID3D11DepthStencilView* m_depthStencilView;

	bool hasDepthStencil;
};