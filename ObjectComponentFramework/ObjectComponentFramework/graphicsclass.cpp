
#include "graphicsclass.h"
#include "Camera.h"
#include "Component.h"
#include "GameObject.h"
#include "Position.h"
#include "Texture.h"
#include "TextureTypes.h"
#include "MeshFactory.h"
#include "Orientation.h"
#include "Material.h"
#include "World.h"
#include <D3D11.h>
#include <D3DX10math.h>
#include "Hardware.h"
#include <initializer_list>

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(HWND hwnd, D3DClass* d3d)
{
	bool result;

		
	m_D3D = d3d;

	shaderLibrary.Initialise(m_D3D->GetDevice(), hwnd);
//	renderTarget.Initialize(m_D3D->GetDevice(), HardwareState::GetInstance().GetScreenWidth(), HardwareState::GetInstance().GetScreenHeight(), 3);
	renderTarget.InitializeTEST(m_D3D->GetDevice(), HardwareState::GetInstance().GetScreenWidth(), HardwareState::GetInstance().GetScreenHeight(), 3, RenderTarget::COLOUR, RenderTarget::NORMAL, RenderTarget::DEPTH);
//	lightingRenderTarget.Initialize(m_D3D->GetDevice(), HardwareState::GetInstance().GetScreenWidth(), HardwareState::GetInstance().GetScreenHeight(), 1);
	lightingRenderTarget.InitializeTEST(m_D3D->GetDevice(), HardwareState::GetInstance().GetScreenWidth(), HardwareState::GetInstance().GetScreenHeight(), 1, RenderTarget::COLOUR);
	m_deferred.Initialize(m_D3D->GetDevice(), hwnd);
	m_final.Initialize(m_D3D->GetDevice(), hwnd);
	m_lighting.Initialize(m_D3D->GetDevice(), hwnd);

///////////////////////////////////////////////////////////////////////////////////////
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	int i;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;



	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// Load the index array with data.
	for(i=0; i<m_indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = m_D3D->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = m_D3D->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Load the vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(-1, 1, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(1, -1, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(-1, -1, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(-1, 1, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(1, 1, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(1, -1, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// Lock the vertex buffer so it can be written to.
	result = m_D3D->GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// Unlock the vertex buffer.
	m_D3D->GetDeviceContext()->Unmap(m_vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	delete [] indices;
	vertices = 0;
	indices = 0;

///////////////////////////////////////////////////////////////////////////////////////


	return true;
}

void GraphicsClass::Shutdown()
{
	return;
}


bool GraphicsClass::Frame(World& world, bool deferred)
{
	bool result;

	// Render the graphics scene.
	if (deferred) {
		result = RenderDeferred(world);
	} else {
		result = Render(world);
	}
	if(!result)
	{
		return false;
	}

	return true;
}



bool GraphicsClass::RenderDeferred(World& world)
{
	bool result;

	std::list<ObjectID> drawList = world.GetDrawList();
	ForwardShaderInterface* currentShader;

	renderTarget.SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	renderTarget.ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	while (drawList.size() > 0) {
		ObjectID current = drawList.back();
		drawList.pop_back();
		result = m_deferred.Render(m_D3D->GetDeviceContext(), current, world.GetCameraObject(), 0);
		if(!result)
		{
			return false;
		}
	}

	lightingRenderTarget.SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());
	lightingRenderTarget.ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexType); 
	unsigned int offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_D3D->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_D3D->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_D3D->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_D3D->EnableLightBlending();
	m_D3D->DisableZBuffer();
	std::list<ObjectID> light = world.GetLightList();

	for (std::list<ObjectID>::iterator it = light.begin(); it != light.end(); it++) {
		m_lighting.Render(m_D3D->GetDeviceContext(), renderTarget, world.GetCameraObject(), (*it));
	}
	m_D3D->DisableLightBlending();
	m_D3D->EnableZBuffer();
	
	m_D3D->SetBackBufferRenderTarget();



	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_final.Render(m_D3D->GetDeviceContext(), lightingRenderTarget);

	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::Render(World& world)
{
	bool result;
	ForwardShaderInterface* currentShader;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	
	std::list<ObjectID> drawList = world.GetDrawList();

	while (drawList.size() > 0) {
		ObjectID current = drawList.back();
		drawList.pop_back();
		GameObject herp = GameObject::Get(current);
		if (current == 2) {
			Material derp = GameObject::GetComponent<Material>(current);
		}
		
		currentShader = shaderLibrary.GetShader(GameObject::GetComponent<Material>(current).GetShader());
		result = currentShader->Render(m_D3D->GetDeviceContext(), current, world.GetCameraObject(), world.GetLight());
		if(!result)
		{
			return false;
		}
	}
	//Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;

}



