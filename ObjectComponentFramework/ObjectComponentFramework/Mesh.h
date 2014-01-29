#pragma once

#include "Component.h"
#include <D3D11.h>
#include <string>

struct MeshData {
	MeshData(){
		m_vertexBuffer = 0;
		m_indexBuffer = 0;
	};
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, stride;
	D3D_PRIMITIVE_TOPOLOGY topology;
};


class Mesh : public Component<Mesh>
{
public:
	enum FeatureLevel {TEXTURED, LIT, NORMALMAPPED};

	Mesh(){};
	~Mesh(){};

	MeshData GetGeometry(){return geometry;}
	void SetMeshData(MeshData data){geometry = data;}
private:
	MeshData geometry;
};