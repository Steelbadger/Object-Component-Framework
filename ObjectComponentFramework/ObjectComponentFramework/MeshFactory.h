#pragma once

#include "Mesh.h"
#include <D3D11.h>
#include <vector>
#include <map>
#include "Vector2.h"
#include "Vector3.h"


#define MODELDIR "Assets/Models/"

class MeshFactory
{
private:
	struct BasicVertexType
	{
		Vector3 position;
		Vector2 texture;
	};

	struct LitVertexType
	{
		Vector3 position;
		Vector2 texture;
		Vector3 normal;
	};

	struct MappedVertexType
	{
		Vector3 position;
		Vector2 texture;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
	};

public:
	enum Primitive {INNERCUBE, OUTERCUBE, INNERSPHERE, OUTERSPHERE, PLANE};

	MeshFactory();
	~MeshFactory();

	MeshData CreateMeshBuffersFromFile(std::string filename, Mesh::FeatureLevel featurelevel);
	MeshData CreatePrimitive(Primitive prim = Primitive::PLANE);
	void SetDevice(ID3D11Device* device);
private:

	MeshData CreateBasicMeshBuffersFromFile(std::string filename);
	MeshData CreateLitMeshBuffersFromFile(std::string filename);
	MeshData CreateMappedMeshBuffersFromFile(std::string filename);

	void LoadBasicVerts(std::string filename, std::vector<BasicVertexType>& outverts, std::vector<unsigned int>& outindex);
	void LoadLitVerts(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex);
	std::vector<MappedVertexType> ComputeTangentSpace(const std::vector<LitVertexType>& data, const std::vector<unsigned int>& index);

	void LoadObj(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex);

	void Plane(std::vector<LitVertexType>& output, std::vector<unsigned int>& indexout, float width, float height, int widthSubDivs, int heightSubDivs);
	void SimpleInnerBox(std::vector<LitVertexType>& output, std::vector<unsigned int>& index);

	ID3D11Device* device;

	std::map<std::string, MeshData> loadedMeshMap;
};