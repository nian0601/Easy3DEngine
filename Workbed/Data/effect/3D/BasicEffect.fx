#include "../SharedUtilities/DirectionalLights.fx"
#include "../SharedUtilities/ShaderVariables.fx"
#include "../SharedUtilities/PointLights.fx"
#include "../SharedUtilities/SpotLights.fx"



SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


PS_INPUT_POS_NORM_TEX_BI_TANG VS(VS_INPUT_POS_NORM_TEX_BI_TANG input)
{
	PS_INPUT_POS_NORM_TEX_BI_TANG output = (PS_INPUT_POS_NORM_TEX_BI_TANG)0;
	
	float4 scale = float4(Scale, 1.0f);
	input.Position *= scale;
	
	input.Position.w = 1.0f;
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	
	output.Tex = input.Tex;
	
	
	output.Normal = mul(input.Normal, World);
	output.BiNormal = input.BiNormal;
	output.Tangent = mul(input.Tangent, World);

	output.WorldPosition = mul(input.Position, World);
	
	return output;
}

float4 PS(PS_INPUT_POS_NORM_TEX_BI_TANG input) : SV_Target
{
	float3 norm = NormalTexture.Sample(linearSampling, input.Tex) * 2 - 1;
	
	input.Normal = normalize(input.Normal);
	input.BiNormal = normalize(input.BiNormal);
	input.Tangent  = normalize(input.Tangent);
	
	float3x3 tangentSpaceMatrix = float3x3(input.Tangent, input.BiNormal, input.Normal);
	norm = normalize(mul(norm, tangentSpaceMatrix));

	float ambient = 1.f;
	
	float4 AlbedoColor = AlbedoTexture.Sample(linearSampling, input.Tex);
	float4 finalColor = AlbedoColor * ambient;
	float4 AmbientOcclusion = AOTexture.Sample(linearSampling, input.Tex);

	
	//DirectionalLight
	for(int i = 0; i < 0; ++i)
	{
		finalColor += CalculateDirectionalLight(DirectionalLights[i], norm);
	}

	//PointLight
	for(int i = 0; i < 0; ++i)
	{
		finalColor += CalculatePointLight(PointLights[i], norm, input.WorldPosition);
	}
	
	//SpotLight
	for(int i = 0; i < 0; ++i)
	{
		finalColor += CalculateSpotLight(SpotLights[i], norm, input.WorldPosition);
	}
	
 
	//CubeMap
	//float4 cubeMap = CubeMap.SampleLevel(linearSampling, norm, 7) * AlbedoColor;
	
	//finalColor += cubeMap;
	//finalColor *= AmbientOcclusion;

	finalColor.a = 1.f;
	
	return finalColor;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}