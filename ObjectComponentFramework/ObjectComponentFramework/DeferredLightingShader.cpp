#include "DeferredLightingShader.h"

#include "Camera.h"
#include "GameObject.h"
#include "Position.h"
#include "DirectionalLight.h"
#include <iostream>
#include "PointLight.h"

#include <D3DX11.h>

DeferredLightingShader::DeferredLightingShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_sampleState = 0;
	m_cameraBuffer = 0;
	m_lightBuffer = 0;
}

DeferredLightingShader::~DeferredLightingShader()
{
}


bool DeferredLightingShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, "deferredLightVertexShader.fx", "deferredLightPixelShader.fx");
	if(!result)
	{
		return false;
	}

	return true;
}


void DeferredLightingShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

bool DeferredLightingShader::Render(ID3D11DeviceContext* deviceContext, RenderTarget& inputData, ObjectID cameraObject, ObjectID lightObject)
{
	bool result;
	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, inputData, cameraObject, lightObject);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, 6);

	return true;
}

bool DeferredLightingShader::InitializeShader(ID3D11Device* device, HWND hwnd, char* vsFilename, char* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, "Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
					   &m_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void DeferredLightingShader::ShutdownShader()
{

		// Release the light constant buffer.
	if(m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the sampler state.
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}


void DeferredLightingShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, char* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();
	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		std::cout << compileErrors[i];
	}

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.", shaderFilename, MB_OK);

	return;
}

bool DeferredLightingShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, RenderTarget& renderTarget, ObjectID cameraObject, ObjectID lightObject)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	LightBufferType* dataPtr2;
	CameraBufferType* dataPtr3;

	// Lock the camera constant buffer so it can be written to.
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (CameraBufferType*)mappedResource.pData;

//	D3DXVECTOR3 cameraPosition = GameObject::GetComponent<Position>(cameraObject).GetPosition();

	D3DXMATRIX projection = GameObject::GetComponent<Camera>(cameraObject).GetProjectionMatrix();
	D3DXMATRIX thingy;
	D3DXMatrixPerspectiveFovLH(&thingy, (45.0f * (float)D3DX_PI)/180.0f, 724.0f/454.0f, 1.0f, 100.0f);

	D3DXVECTOR4 frustumCorners[8] = {	D3DXVECTOR4(-1, 1, 0, 1), D3DXVECTOR4(1, 1, 0, 1), D3DXVECTOR4(1, -1, 0, 1), D3DXVECTOR4(-1, -1, 0, 1),
										D3DXVECTOR4(-1, 1, 1, 1), D3DXVECTOR4(1, 1, 1, 1), D3DXVECTOR4(1, -1, 1, 1), D3DXVECTOR4(-1, -1, 1, 1)};



	D3DXMATRIX view = GameObject::GetComponent<Camera>(cameraObject).GetViewMatrix();
	D3DXMATRIX invProjection;
	float det;
	D3DXMatrixInverse(&invProjection, &det, &projection);
	invProjection = invProjection/det;
//	D3DXMatrixTranspose(&invProjection, &invProjection);

	D3DXVECTOR4 test1(0, 0, 10, 1);

	D3DXVec4Transform(&test1, &test1, &projection);
	test1 = test1/test1.w;
	D3DXVec4Transform(&test1, &test1, &invProjection);
	test1 = test1/test1.w;


//  This is bizarre, 4 float4s apparently get treated as a matrix and thus need to be transposed for use in the shader?  Wat?
	D3DXVec4TransformArray(frustumCorners, 16, frustumCorners, 16, &invProjection, 8);
	dataPtr3->topLeft = (frustumCorners[4]/frustumCorners[4].w) - (frustumCorners[0]/frustumCorners[0].w);
	dataPtr3->topRight = (frustumCorners[5]/frustumCorners[5].w) - (frustumCorners[1]/frustumCorners[1].w);
	dataPtr3->bottomLeft = (frustumCorners[7]/frustumCorners[7].w) - (frustumCorners[3]/frustumCorners[3].w);
	dataPtr3->bottomRight = (frustumCorners[6]/frustumCorners[6].w) - (frustumCorners[2]/frustumCorners[2].w);

	dataPtr3->topLeft.w = 1.0f;
	dataPtr3->topRight.w = 1.0f;
	dataPtr3->bottomLeft.w = 1.0f;
	dataPtr3->bottomRight.w = 1.0f;

//	dataPtr3->topLeft = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXMatrixTranspose(&invProjection, &invProjection);

	// Copy the camera position into the constant buffer.
//	dataPtr3->invProj = invProjection;

	// Unlock the camera constant buffer.
	deviceContext->Unmap(m_cameraBuffer, 0);

	// Set the position of the camera constant buffer in the vertex shader.
	bufferNumber = 1;

	// Now set the camera constant buffer in the vertex shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	//  Get the render target and map it as input.
	std::vector<ID3D11ShaderResourceView*> textures = renderTarget.GetShaderResourceViews();

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, textures.size(), textures.data());

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.

	D3DXVECTOR4 position;
	Position* lightPos = &GameObject::GetComponent<Position>(lightObject);
	if (GameObject::HasComponent<DirectionalLight>(lightObject)) {
		D3DXVECTOR4 direction = D3DXVECTOR4(GameObject::GetComponent<DirectionalLight>(lightObject).GetDirection(), 1.0f);
		float det;
		view._14 = 0;
		view._24 = 0;
		view._34 = 0;
		view._41 = 0;
		view._42 = 0;
		view._43 = 0;
		view._44 = 1;

		D3DXMatrixTranspose(&view, &view);
		D3DXMatrixInverse(&view, &det, &view);
		view = view/det;

		D3DXVec4Transform(&direction, &direction, &view);
		direction = direction/direction.w;
		direction.w = 0;
		D3DXVec4Normalize(&direction, &direction);

		dataPtr2->lightDirection = direction;
		dataPtr2->lightColor = GameObject::GetComponent<DirectionalLight>(lightObject).GetColour();
		dataPtr2->specularPower = GameObject::GetComponent<DirectionalLight>(lightObject).GetSpecularPower();
	} else if (GameObject::HasComponent<PointLight>(lightObject)){

		position = D3DXVECTOR4(GameObject::GetComponent<PointLight>(lightObject).GetPosition(), 1.0f);
		D3DXVec4Transform(&position, &position, &view);
		position = position/position.w;

		dataPtr2->lightDirection = position;
		dataPtr2->lightColor = GameObject::GetComponent<PointLight>(lightObject).GetColour();
		dataPtr2->specularPower = GameObject::GetComponent<PointLight>(lightObject).GetSpecularPower();
	}

	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

void DeferredLightingShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

