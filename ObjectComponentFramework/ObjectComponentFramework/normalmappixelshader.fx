////////////////////////////////////////////////////////////////////////////////
// Filename: normalmappixelshader.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////

Texture2D shaderTextures[3];
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
	float specularPower;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
	float3 worldPosition : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 NormalMapPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 fragToLight;
	float3 fragToView;
    float4 color;
	float fac = 0.3;

	float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
	float4 ambient = float4(0.0, 0.0, 0.0, 0.0);
	float4 specular = float4(0.0, 0.0, 0.0, 0.0);
	float4 lightcolour = float4(0.0, 0.0, 0.0, 0.0);

    // Sample the texture pixel at this location.
    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	float specularity = shaderTextures[2].Sample(SampleType, input.tex);
	//textureColor = shaderTextures[1].Sample(SampleType, input.tex);
	color = diffuseColor;

	ambient = ambient + (textureColor * diffuseColor * fac);
		
    // Sample the pixel in the bump map.
    bumpMap = shaderTextures[1].Sample(SampleType, input.tex);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the bump map.
    bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;
	
    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);

    // Invert the light direction for calculations.
    fragToLight = normalize(-lightDirection);
	fragToView = normalize(cameraPosition - input.worldPosition);

	float diffuseContribution = max(0.0, dot(bumpNormal, fragToLight));

	float attenuation = 1;

	diffuse = diffuse + (textureColor * diffuseColor * diffuseContribution * attenuation);

	float3 lightReflection = reflect(fragToLight, bumpNormal);
	bumpNormal = normalize(bumpNormal);

	float specularContribution = max(0.0, dot(-lightReflection, fragToView));

	specularContribution = pow(specularContribution, specularPower)*specularity;

	specular = specular + (textureColor * diffuseColor * specularContribution * attenuation);

	color = ambient + diffuse + specular;

	float cdepth = length(cameraPosition - input.worldPosition)/100;
	float3 vect = normalize(input.worldPosition);

//	color = float4(diffuseContribution, diffuseContribution, diffuseContribution, 1.0f);
//	color = float4(bumpNormal, 1.0f);
//	color = float4(specularContribution, specularContribution, specularContribution, 1.0f);
//	color = float4(specularity, specularity, specularity, 1.0f);
//	color = float4(lightReflection.x, lightReflection.x, lightReflection.x, 1.0f);
//	color = float4(lightReflection, 1.0f);
//	color = float4(fragToLight, 1.0f);
//	color = float4(fragToView, 1.0f);
//	color = float4(cdepth, cdepth, cdepth, 1.0f);
//	color = float4(input.worldPosition, 1.0f);
//	color = float4(vect, 1.0f);

    return color;
}
