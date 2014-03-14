#pragma once

#include "Component.h"
#include <D3D11.h>
#include <string>
#include <memory>

struct MeshData {
	MeshData(){
		m_vertexBuffer = 0;
		m_indexBuffer = 0;
	};
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount, stride;
	D3D_PRIMITIVE_TOPOLOGY topology;
};


class Mesh : public rabd::Component<Mesh>
{
public:
	enum FeatureLevel {TEXTURED, LIT, NORMALMAPPED};

	Mesh(){};
//	Mesh(Mesh&& move){geom = move.geom;}
	~Mesh(){};

	std::shared_ptr<MeshData>& GetGeometry(){return geom;}
	void SetMeshData(std::shared_ptr<MeshData>& data){geom = data;}

//	Mesh& operator=(Mesh&& move){geom = move.geom; return (*this);}

private:
	std::shared_ptr<MeshData> geom;
};