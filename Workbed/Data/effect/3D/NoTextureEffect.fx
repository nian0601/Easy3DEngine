#include "../SharedUtilities/DirectionalLights.fx"
#include "../SharedUtilities/ShaderVariables.fx"
#include "../SharedUtilities/PointLights.fx"
#include "../SharedUtilities/SpotLights.fx"

matrix Bones[64];


struct VS_ANIMATION_INPUT
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 Bin: BINORMAL;
	float4 Tan : TANGENT;
	uint4 Bones : BONES;
	float4 Weights : WEIGHTS;
};

struct SkinnedVert
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

SkinnedVert VS(VS_ANIMATION_INPUT input)
{
	SkinnedVert output = (SkinnedVert)0;

	float4 pos = input.Position;
	float3 norm = input.Normal;
	
	uint iBone;
	float fWeight;

	// Bone 0
	iBone = input.Bones.x;
	fWeight = input.Weights.x;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);

	// Bone 1
	iBone = input.Bones.y;
	fWeight = input.Weights.y;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);

	// Bone 2
	iBone = input.Bones.z;
	fWeight = input.Weights.z;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);

	// Bone 3
	iBone = input.Bones.w;
	fWeight = input.Weights.w;

	output.Position += fWeight * mul(pos, Bones[iBone]);
	output.Normal += fWeight * mul(norm, (float3x3)Bones[iBone]);


	output.Position = mul(output.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	return output;
}

float4 PS(SkinnedVert input) : SV_Target
{
	return float4(1.0f, 0.6f, 0.6f, 1.f);
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


/*

PS_INPUT_POS_NORM_TEX_BI_TANG VS(VS_INPUT_POS_NORM_TEX_BI_TANG input)
{
	PS_INPUT_POS_NORM_TEX_BI_TANG output = (PS_INPUT_POS_NORM_TEX_BI_TANG)0;
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

	float ambient = 0.3f;
	
	float4 finalColor = float4(ambient, ambient, ambient, 1.f);

	
	//DirectionalLight
	for(int i = 0; i < 1; ++i)
	{
		finalColor += CalculateDirectionalLight(DirectionalLights[i], norm);
	}
	
	//PointLight
	for(int i = 0; i < 1; ++i)
	{
		finalColor += CalculatePointLight(PointLights[i], norm, input.WorldPosition);
	}
	
	//SpotLight
	for(int i = 0; i < 0; ++i)
	{
		finalColor += CalculateSpotLight(SpotLights[i], norm, input.WorldPosition);
	}
	
	float4 cubeMap = CubeMap.SampleLevel(linearSampling, norm, 7);
	
	finalColor += cubeMap;
	finalColor.a = 1.f;
	
	return finalColor;
}

*/