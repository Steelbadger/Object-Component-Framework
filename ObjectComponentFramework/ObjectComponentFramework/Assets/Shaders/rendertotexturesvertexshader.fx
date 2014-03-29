////////////////////////////////////////////////////////////////////////////////
// Filename: bumpmap.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType AmbientNormalShader(VertexInputType input)
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
	float dist;
	float4 inv = float4(0.0f,0.0f,0.0f,1.0f);
	inv = mul(inv, viewMatrix);
	inv = -inv;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
	//output.position = mul(vertPos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
	dist = output.position.z/output.position.w;

	// TESTING
	// Spherize
	float groundPlane = -1000.0f;

	float vertR = input.position.y - groundPlane;
	float playerR = inv - groundPlane;

	float dist2 = length(output.position.xz);

	float yChange = groundPlane + sqrt(pow(vertR,2) - pow(dist2, 2));

	float4 vertPos = input.position;
	output.position.y += yChange*output.position.w;

	float zcorr = sqrt(pow(input.position.y,2) - pow(yChange, 2));

	output.position.z += zcorr*output.position.w;

	// DONE TESTING


	dist = dist/1000.0f;
    output.position = mul(output.position, projectionMatrix);

	dist = dist * output.position.w;
	output.position.z = dist;

	matrix worldView = mul(worldMatrix, viewMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only and then normalize the final value.
    output.normal = mul(input.normal, (float3x3)worldView);
    output.normal = normalize(output.normal);

    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.tangent = mul(input.tangent, (float3x3)worldView);
    output.tangent = normalize(output.tangent);

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    output.binormal = mul(input.binormal, (float3x3)worldView);
    output.binormal = normalize(output.binormal);

    return output;
}
