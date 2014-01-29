////////////////////////////////////////////////////////////////////////////////
// Filename: normalmappixelshader.fx
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////

Texture2D shaderTextures[3];
SamplerState SampleType;

cbuffer LightBuffer : register(b0)
{
    float4 diffuseColor;
    float4 lightDirection;
    float specularPower;
	float3 lightPadding;
};

cbuffer CameraBuffer
{
//	matrix invProj;
	float4 topLeft;
	float4 topRight;
	float4 bottomLeft;
	float4 bottomRight;
}

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

	float4 diffuse = float4(0.0, 0.0, 0.0, 0.0);
	float4 ambient = float4(0.0, 0.0, 0.0, 0.0);
	float4 specular = float4(0.0, 0.0, 0.0, 0.0);

    textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	normalMap = shaderTextures[1].Sample(SampleType, input.tex);
	float specularity = normalMap.w;
//	normal = normalize((normalMap.xyz*2.0f)-1.0f);
	normal = normalMap.xyz;
//	normal = normalize(normal);

	float cdepth = shaderTextures[2].Sample(SampleType, input.tex).x;
	float depth = cdepth*100.0f;

	float3 horiz1 = normalize(lerp(topLeft.xyz, topRight.xyz, input.tex.x));
	float3 horiz2 = normalize(lerp(bottomLeft.xyz, bottomRight.xyz, input.tex.x));

	float3 vect = lerp(horiz2, horiz1, input.tex.y);

	vect = normalize(vect);
	vect.y = vect.y*-1;
//	vect.z = vect.z*-1;

//	float4 projectionPosition = float4(input.tex.x*2-1, (1-input.tex.y)*2-1, depth, 1.0f);
//	pixelPosition = mul(projectionPosition, invProj);
//	pixelPosition = pixelPosition/pixelPosition.w;

	pixelPosition = float4(vect*depth, 1);


	float4 col = textureColor * diffuseColor;


	// Invert the light direction for calculations.
	float3 fragToLight = normalize(lightDirection.xyz - (pixelPosition.xyz * lightDirection.w));
	fragToLight = ((lightDirection.w*2)-1)*fragToLight;

	float3 fragToView = normalize(-pixelPosition.xyz);

	float dist = distance(lightDirection.xyz, pixelPosition) * lightDirection.w;

	//ambient = textureColor * diffuseColor * fac * (1-lightDirection.w);	
	ambient = fac * (1-lightDirection.w);

	float diffuseContribution = max(0.0, dot(normal.xyz, fragToLight));

	float attenuation = (1/(1 + 0.1*dist + 0.01 * dist * dist));

	//diffuse = textureColor * diffuseColor * diffuseContribution * attenuation;
	diffuse = diffuseContribution * attenuation;

	float3 lightReflection = reflect(fragToLight, normal);

	float specularContribution = max(0.0, dot(-lightReflection, fragToView));

	specularContribution = pow(specularContribution, specularPower) * specularity;

	//specular = textureColor * diffuseColor * specularContribution * attenuation;
	specular = specularContribution * attenuation;

	//  Don't apply lighting to areas with no geometry (normal.w will be set to 1.0 in those areas)
	color = col * (ambient + diffuse + specular);
//	color = (projectionPosition+1)/2;
//	color = float4(pixelPosition.xyz, 1.0f);
//	color = float4(dist, dist, dist, 1.0f)/100;
//	color = float4(diffuseContribution, diffuseContribution, diffuseContribution, 1.0f);
//	color = float4(specularContribution, specularContribution, specularContribution, 1.0f);
//	color = float4(specularity, specularity, specularity, 1.0f);
//	color = float4(fragToLight, 1.0f);
//	color = float4(fragToView, 1.0f);
//	color = float4(lightReflection.x, lightReflection.x, lightReflection.x, 1.0f);
//	color = float4(lightReflection, 1.0f);
//	color = textureColor;
//	color = float4(normal.xyz, 1.0f);
//	color = float4(normalMap.xyz, 1.0f);
//	color = float4(horiz1, 1.0f);
//	color = float4(lightDirection.xyz, 1.0f);

//	color = float4(topLeft.xyz, 1.0f);			//RED?
//	color = float4(topRight.xyz, 1.0f);			//BLACK?????
//	color = float4(bottomLeft.xyz, 1.0f);		//YELLOW?
//	color = float4(bottomRight.xyz, 1.0f);		//GREEN?

//	color = float4(64, -41, -99, 1.0f);
//	color = float4(normal.y, normal.y, normal.y, 1.0f);
//	color = float4(input.tex, 0.0f, 1.0f);
//	color = float4(horiz1, 1.0f);
//	color = float4(vect, 1.0f);
//	color = float4(cdepth, cdepth, cdepth, 1.0f);
//	color = float4(depth/100, depth/100, depth/100, 1.0f);

    return color;
}
