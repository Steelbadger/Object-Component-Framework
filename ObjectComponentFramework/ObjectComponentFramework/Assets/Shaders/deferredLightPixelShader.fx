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
    float4 lightDirection;
    float specularPower;
	float3 topRight;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
	float4 normalMap;
	float3 normal;
	float4 pixelPosition;
	float4 color;
	float fac = 0.3;

	float3 topLeft = topRight * float3(-1, 1, 1);
	float3 bottomLeft = topRight * float3(-1, -1, 1);
	float3 bottomRight = topRight * float3(1, -1, 1);

	float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
	float4 ambient = float4(0.0, 0.0, 0.0, 0.0);
	float4 specular = float4(0.0, 0.0, 0.0, 0.0);

    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	normalMap = shaderTextures[1].Sample(SampleType, input.tex);
	float specularity = normalMap.w;
	normal = normalize((normalMap.xyz*2.0f)-1.0f);
//	normal = normalize(normal);

	float cdepth = shaderTextures[2].Sample(SampleType, input.tex).x;
	float depth = cdepth*1000.0f;

	float3 horiz1 = lerp(topLeft, topRight, input.tex.x);
	float3 horiz2 = lerp(bottomLeft, bottomRight, input.tex.x);

	float3 vect = lerp(horiz1, horiz2, input.tex.y);

//	vect = normalize(vect);
//	float vectL = length(vect);

	pixelPosition = float4((vect*depth), 1);

	float4 col = textureColor * diffuseColor;

	// Invert the light direction for calculations.
	float3 fragToLight = normalize(lightDirection.xyz - (pixelPosition.xyz * lightDirection.w));
	fragToLight = ((lightDirection.w*2)-1)*fragToLight;

	float3 fragToView = normalize(-pixelPosition.xyz);

	float dist = distance(lightDirection.xyz, pixelPosition) * lightDirection.w;


	ambient = fac * (1-lightDirection.w);

	float diffuseContribution = max(0.0, dot(normal.xyz, fragToLight));

	float attenuation = (1/(1 + 0.1*dist + 0.01 * dist * dist));

	diffuse = diffuseContribution * attenuation;

	float3 lightReflection = reflect(fragToLight, normal);

	float specularContribution = max(0.0, dot(-lightReflection, fragToView));

	specularContribution = pow(specularContribution, specularPower) * specularity;

	specular = specularContribution * attenuation;

	//  Don't apply lighting to areas with no geometry (normal.w will be set to 1.0 in those areas)
	color = col * (ambient + diffuse + specular);
//	color = float4(normalMap.x, normalMap.y, normalMap.z, 1.0f);
//	color = float4(diffuseContribution, diffuseContribution, diffuseContribution, 1.0f);
//	color = float4((lightDirection.xyz+1)/2, 1.0f);
//	color = pixelPosition/10.0f;
//	color = float4(vect, 1.0f);
//	color = float4(specularContribution, specularContribution, specularContribution, 1.0f);
//	color = float4(specularity, specularity, specularity, 1.0f);
//	color = float4(specularPower, specularPower, specularPower, 1.0f);
//	color = float4(lightReflection, 1.0f);
//	color = float4(fragToView, 1.0f);
//	color = float4(topLeft, 1.0f);
//	color = float4(topRight, 1.0f);
//	color = float4(bottomLeft, 1.0f);
//	color = float4(bottomRight, 1.0f);
//	color = float4(cdepth, cdepth, cdepth, 1.0f);
//	color = float4(input.tex.x, input.tex.y, 0.0f, 1.0f);
//	color = float4(horiz1, 1.0f);
//	color = float4(vectL, vectL, vectL, 1.0f);
//	color = float4(dist, dist, dist, 1.0f);
//	color = float4(attenuation, attenuation, attenuation, 1.0f);
//	color = float4(lightDirection.www, 1.0f);
//	color = float4(vect.xy, cdepth, 1.0f);

    return color;
}
