#pragma once
#include <D3D11.h>
#include <D3DX10math.h>
#include "GameObject.h"
#include "RenderTarget.h"
#include "World.h"
#include <vector>

namespace ShaderTypes
{
	struct Domain
	{

	};

	struct Geometry
	{

	};

	struct Vertex
	{

	};

	struct Pixel
	{

	};
};


template<class ShaderStage, class ConstantBufferType>
class TestShaderType
{
public:
	TestShaderType(){}
	~TestShaderType(){}
private:
	ID3D11DeviceChild* m_shader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_cBuffer;

};

template<class ConstantBufferType>
class TestShaderType<ShaderTypes::Pixel, ConstantBufferType>
{
public:
	TestShaderType(){}
	~TestShaderType(){}



private:
	ID3D11PixelShader* m_shader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_cBuffer;
};
