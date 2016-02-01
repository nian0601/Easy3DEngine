matrix World;
matrix View;
matrix Projection;

Texture2D DiffuseTexture;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
};


struct VS_INPUT_POS_NORM_TEX
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_NORM_TEX
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};


PS_INPUT_POS_NORM_TEX VS(VS_INPUT_POS_NORM_TEX input)
{
	PS_INPUT_POS_NORM_TEX output = (PS_INPUT_POS_NORM_TEX)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;
	output.Norm = mul(input.Norm, World);
	
	return output;
}

float4 PS(PS_INPUT_POS_NORM_TEX input) : SV_Target
{
	float ambient = 0.1f;
	float4 finalColor = DiffuseTexture.Sample(linearSampling, input.Tex) * ambient;
	
	finalColor.a = 1;
	
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