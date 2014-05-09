#include "MeshFactory.h"
#include "UtilityFunctions.h"
#include <memory>
#include "noisegenerator.h"
#include <ppl.h>
#include <array>

MeshFactory::MeshFactory()
{
}


MeshFactory::~MeshFactory()
{
}

void MeshFactory::SetDevice(ID3D11Device* dev)
{
	device = dev;
}

std::shared_ptr<MeshData>& MeshFactory::CreateMeshBuffersFromFile(std::string filename, Mesh::FeatureLevel features)
{
	std::shared_ptr<MeshData> output;
	std::string lookup = filename + std::to_string(unsigned long long(features));

	if (loadedMeshMap.count(lookup) == 0) {
		switch (features) {
		case Mesh::FeatureLevel::TEXTURED:
			output = std::shared_ptr<MeshData>(new MeshData(CreateBasicMeshBuffersFromFile(filename)));
			break;
		case Mesh::FeatureLevel::LIT:
			output = std::shared_ptr<MeshData>(new MeshData(CreateLitMeshBuffersFromFile(filename)));
			break;
		case Mesh::FeatureLevel::NORMALMAPPED:
			output = std::shared_ptr<MeshData>(new MeshData(CreateMappedMeshBuffersFromFile(filename)));
			break;
		}
		loadedMeshMap[lookup] = output;
	}
	return loadedMeshMap[lookup];
}

std::shared_ptr<MeshData>& MeshFactory::CreatePrimitive(int heightType)
{
	std::vector<LitVertexType> vertsLoad;
	std::vector<MappedVertexType> verts;
	std::vector<unsigned int> index;
	std::string lookup = std::to_string(unsigned long long(heightType));

	if (loadedMeshMap.count(lookup) == 0) {
		Plane(vertsLoad, index, 1000.0f, 1000.0f, 400, 400);

		switch (heightType) {
		case 0:
			ApplyDisplacement(vertsLoad, index);
			break;
		case 1:
			ApplyDisplacementCoherent(vertsLoad, index);
			break;
		case 2:
			ApplyDisplacementSIMD(vertsLoad, index);
			break;
		case 3:
			ApplyDisplacementSimplex(vertsLoad, index);
			break;
		}
		verts = ComputeTangentSpace(vertsLoad, index);

		MeshData output;

		output.m_vertexCount = verts.size();
		output.m_indexCount = index.size();
		output.stride = sizeof(MappedVertexType);
		output.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(MappedVertexType) * output.m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = &verts[0];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &output.m_vertexBuffer);
		if(FAILED(result))
		{
			Error("Failed to Create Vertex Buffer for file: Primitive Plane");
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * output.m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = &index[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &output.m_indexBuffer);
		if(FAILED(result))
		{
			Error("Failed to Create Index Buffer for file: Primitive Plane");
		}

		loadedMeshMap[lookup] = std::shared_ptr<MeshData>(new MeshData(output));
	}
	return loadedMeshMap[lookup];
}

MeshData MeshFactory::CreateBasicMeshBuffersFromFile(std::string filename)
{
	std::vector<BasicVertexType> verts;
	std::vector<unsigned int> index;
	LoadBasicVerts(filename, verts, index);

	MeshData output;

	output.stride = sizeof(BasicVertexType);

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	output.m_indexCount = index.size();
	output.m_vertexCount = verts.size();

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(BasicVertexType) * output.m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &verts[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &output.m_vertexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Vertex Buffer for file: " + filename);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * output.m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &index[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &output.m_indexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Index Buffer for file: " + filename);
	}

	return output;
}

MeshData MeshFactory::CreateLitMeshBuffersFromFile(std::string filename)
{
	std::vector<LitVertexType> verts;
	std::vector<unsigned int> index;
	LoadLitVerts(filename, verts, index);

	MeshData output;

	output.m_vertexCount = verts.size();
	output.m_indexCount = index.size();
	output.stride = sizeof(LitVertexType);
	output.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(LitVertexType) * output.m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &verts[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &output.m_vertexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Vertex Buffer for file: " + filename);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * output.m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &index[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &output.m_indexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Index Buffer for file: " + filename);
	}

	return output;
}

MeshData MeshFactory::CreateMappedMeshBuffersFromFile(std::string filename)
{
	std::vector<LitVertexType> vertsLoad;
	std::vector<MappedVertexType> verts;
	std::vector<unsigned int> index;
	LoadLitVerts(filename, vertsLoad, index);

	verts = ComputeTangentSpace(vertsLoad, index);

	MeshData output;

	output.m_vertexCount = verts.size();
	output.m_indexCount = index.size();
	output.stride = sizeof(MappedVertexType);
	output.topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(MappedVertexType) * output.m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = &verts[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &output.m_vertexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Vertex Buffer for file: " + filename);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * output.m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = &index[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &output.m_indexBuffer);
	if(FAILED(result))
	{
		Error("Failed to Create Index Buffer for file: " + filename);
	}

	return output;
}

void MeshFactory::LoadBasicVerts(std::string filename, std::vector<BasicVertexType>& outverts, std::vector<unsigned int>& outindex)
{
	std::vector<BasicVertexType> result;
	std::vector<LitVertexType> loaded;
	std::vector<unsigned int> index;

	LoadObj(filename, loaded, index);

	for (int i = 0; i < loaded.size(); i++) {
		BasicVertexType vert;
		vert.position = loaded[i].position;
		vert.texture = loaded[i].texture;
		result.push_back(vert);
	}


}

void MeshFactory::LoadLitVerts(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex)
{
	LoadObj(filename, outverts, outindex);
}

std::vector<MeshFactory::MappedVertexType> MeshFactory::ComputeTangentSpace(const std::vector<LitVertexType>& data, const std::vector<unsigned int>& index)
{
	std::vector<MappedVertexType> output;
	output.resize(data.size());

	TimerMark();

	using namespace Concurrency;

	parallel_for(size_t(0), (index.size()/3), [&](int it) {
		int i = it*3;
		LitVertexType vertex1, vertex2, vertex3;
		vertex1 = data[index[i]];
		vertex2 = data[index[i+1]];
		vertex3 = data[index[i+2]];
		Vector3 tangent;
		Vector3 binormal;

		float vector1[3], vector2[3];
		float tuVector[2], tvVector[2];
		float den;
		float length;

		// Calculate the two vectors for this face.
		vector1[0] = vertex2.position.x - vertex1.position.x;
		vector1[1] = vertex2.position.y - vertex1.position.y;
		vector1[2] = vertex2.position.z - vertex1.position.z;

		vector2[0] = vertex3.position.x - vertex1.position.x;
		vector2[1] = vertex3.position.y - vertex1.position.y;
		vector2[2] = vertex3.position.z - vertex1.position.z;

		// Calculate the tu and tv texture space vectors.
		tuVector[0] = vertex2.texture.x - vertex1.texture.y;
		tvVector[0] = vertex2.texture.y - vertex1.texture.y;

		tuVector[1] = vertex3.texture.x - vertex1.texture.x;
		tvVector[1] = vertex3.texture.y - vertex1.texture.y;

		// Calculate the denominator of the tangent/binormal equation.
		den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

		// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
		tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
		tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
		tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

		binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
		binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
		binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

		// Calculate the length of this normal.
		length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
		// Normalize the normal and then store it
		tangent.x = tangent.x / length;
		tangent.y = tangent.y / length;
		tangent.z = tangent.z / length;

		// Calculate the length of this normal.
		length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
		// Normalize the normal and then store it
		binormal.x = binormal.x / length;
		binormal.y = binormal.y / length;
		binormal.z = binormal.z / length;

		MappedVertexType mVertex1, mVertex2, mVertex3;
		mVertex1.position = vertex1.position;
		mVertex1.normal = vertex1.normal;
		mVertex1.texture = vertex1.texture;
		mVertex1.tangent = tangent;
		mVertex1.bitangent = binormal;

		mVertex2.position = vertex2.position;
		mVertex2.normal = vertex2.normal;
		mVertex2.texture = vertex2.texture;
		mVertex2.tangent = tangent;
		mVertex2.bitangent = binormal;

		mVertex3.position = vertex3.position;
		mVertex3.normal = vertex3.normal;
		mVertex3.texture = vertex3.texture;
		mVertex3.tangent = tangent;
		mVertex3.bitangent = binormal;

		output[index[i]] = mVertex1;
		output[index[i+1]] = mVertex2;
		output[index[i+2]] = mVertex3;
	});

	TimerMark(true);

	return output;
}

void MeshFactory::LoadObj(std::string filename, std::vector<LitVertexType>& outverts, std::vector<unsigned int>& outindex)
{
	std::vector<unsigned int> vertIndices, uvIndices, normalIndices;
	std::vector<Vector3> tempVerts;
	std::vector<Vector3> tempNormals;
	std::vector<Vector2> tempUVs;

	std::vector<unsigned int> index;
	std::vector<LitVertexType> vertexs;

	std::string longName = MODELDIR + filename;

	const char* path = longName.c_str();

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		Error("Cannot Open File: " + filename);
		return;
	}

	while(true){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVerts.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			tempUVs.push_back(uv);
		} else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		} else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				Error("Cannot Read File: " + filename);
				fclose(file);
				return;
			}
			vertIndices.push_back(vertexIndex[0]);
			vertIndices.push_back(vertexIndex[1]);
			vertIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		} else {
			// Probably a comment, eat up the rest of the line
			char stuff[1000];
			fgets(stuff, 1000, file);
		}

	}
	fclose(file);

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		LitVertexType v;
		v.position = tempVerts[vertexIndex-1];;
		v.texture = tempUVs[uvIndex-1];
		v.normal = tempNormals[normalIndex-1];

		// Put the attributes in buffers
		vertexs.push_back(v);
		index.push_back(i);
	}

	outverts = vertexs;
	outindex = index;
}

void MeshFactory::Plane(std::vector<LitVertexType>& output, std::vector<unsigned int>& index, float width, float height, int widthSubDivs, int heightSubDivs)
{
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	index.clear();

	float widthStep = width/(widthSubDivs-1);
	float heightStep = height/(heightSubDivs-1);

	int indexcount = 0;
	for (int i = 0; i < heightSubDivs-1; i++) {
		for (int j = 0; j < widthSubDivs-1; j++) {
			verts.push_back(Vector3(i*widthStep,0, j*heightStep));
			texCoords.push_back(Vector2(0,0));
			verts.push_back(Vector3(i*widthStep,0, (j+1)*heightStep));
			texCoords.push_back(Vector2(0,1));
			verts.push_back(Vector3((i+1)*widthStep,0, (j+1)*heightStep));
			texCoords.push_back(Vector2(1,1));

			verts.push_back(Vector3(i*widthStep,0, j*heightStep));
			texCoords.push_back(Vector2(0,0));
			verts.push_back(Vector3((i+1)*widthStep,0, (j+1)*heightStep));
			texCoords.push_back(Vector2(1,1));
			verts.push_back(Vector3((i+1)*widthStep,0, j*heightStep));
			texCoords.push_back(Vector2(1,0));

			for (int k = 0; k < 6; k++) {
				index.push_back(indexcount++);
				normals.push_back(Vector3(0,1,0));
			}
		}
	}

	for (int i = 0; i < verts.size(); i++) {
		LitVertexType temp;
		temp.position = verts[i];
		temp.normal = normals[i];
		temp.texture = texCoords[i];
		output.push_back(temp);
	}
}

void MeshFactory::ApplyDisplacement(std::vector<LitVertexType>& inoutV, std::vector<unsigned int>& inoutIndex)
{
	NoiseGenerator noise;
	NoiseObject n(8, 200.0f, 0.41f, 80.0f, 1.155f);
	noise.Seed(13.4f);
//	noise.GeneratePermutationTable();

	using namespace Concurrency;
	std::cout << "Time to Generate Improved Perlin Heights: ";
	TimerMark();

	parallel_for(size_t(0), inoutIndex.size()-1, [&](int i) {
		float x, y;
		x = inoutV[inoutIndex[i]].position.x;
		y = inoutV[inoutIndex[i]].position.z;


		float disp = (noise.ImprovedFractal2DPerlin(x, y, n)/noise.MaxAmplitude(n))*n.amplitude;
		Vector3 displacementV = disp * Vector3(0, 1, 0);
		inoutV[inoutIndex[i]].position.y = disp;
		inoutV[inoutIndex[i]].normal = noise.ImprovedPerlinNormal(x, y, n, 0.5f);
	});

	TimerMark(true);
}

void MeshFactory::ApplyDisplacementSimplex(std::vector<LitVertexType>& inoutV, std::vector<unsigned int>& inoutIndex)
{
	NoiseGenerator noise;
	NoiseObject n(8, 200.0f, 0.41f, 80.0f, 1.155f);
	noise.Seed(12.3f);
//	noise.GeneratePermutationTable();

	using namespace Concurrency;
	std::cout << "Time to Generate Simplex Heights: ";
	TimerMark();

	parallel_for(size_t(0), inoutIndex.size()-1, [&](int i) {
		float x, y;
		x = inoutV[inoutIndex[i]].position.x;
		y = inoutV[inoutIndex[i]].position.z;


		float disp = noise.FractalSimplex(x, y, n);
		Vector3 displacementV = disp * Vector3(0, 1, 0);
		inoutV[inoutIndex[i]].position.y = disp;
		inoutV[inoutIndex[i]].normal = noise.FractalSimplexNormal(x, y, n, 0.5f);
	});

	TimerMark(true);
}

void MeshFactory::ApplyDisplacementSIMD(std::vector<LitVertexType>& inoutV, std::vector<unsigned int>& inoutIndex)
{
	NoiseGenerator noise;
	NoiseObject n(8, 200.0f, 0.41f, 80.0f, 1.155f);
	noise.Seed(13.4f);
//	noise.GeneratePermutationTable();

	using namespace Concurrency;
	std::cout << "Time to Generate SIMD Accelerated Coherent Heights: ";
	TimerMark();

	parallel_for(size_t(0), inoutIndex.size()-1, [&](int i) {
		float x, y;
		x = inoutV[inoutIndex[i]].position.x;
		y = inoutV[inoutIndex[i]].position.z;


		float disp = (noise.SIMDCoherentFractalNoise(x, y, n)/noise.MaxAmplitude(n))*n.amplitude;
		Vector3 displacementV = disp * Vector3(0, 1, 0);
		inoutV[inoutIndex[i]].position.y = disp;
		inoutV[inoutIndex[i]].normal = noise.SIMDCoherentNoiseNormal(x, y, n, 0.5f);
	});

	TimerMark(true);
}

void MeshFactory::ApplyDisplacementCoherent(std::vector<LitVertexType>& inoutV, std::vector<unsigned int>& inoutIndex)
{
	NoiseGenerator noise;
	NoiseObject n(8, 200.0f, 0.41f, 80.0f, 1.155f);
	noise.Seed(13.4f);
//	noise.GeneratePermutationTable();

	using namespace Concurrency;

	TimerMark();

	parallel_for(size_t(0), inoutIndex.size()-1, [&](int i) {
		float x, y;
		x = inoutV[inoutIndex[i]].position.x;
		y = inoutV[inoutIndex[i]].position.z;


		float disp = noise.FractalCoherentNoise(x, y, n);
		Vector3 displacementV = disp * Vector3(0, 1, 0);
		inoutV[inoutIndex[i]].position.y = disp;
		inoutV[inoutIndex[i]].normal = noise.CoherentNoiseNormal(x, y, n, 0.5f);
	});

	TimerMark(true);
}
