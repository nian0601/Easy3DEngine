#include "../SharedUtilities/ShaderVariables.fx"

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

PS_INPUT_POS_NORM_TEX VS(VS_INPUT_POS_NORM_TEX input)
{
	PS_INPUT_POS_NORM_TEX output = (PS_INPUT_POS_NORM_TEX)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	//output.Pos = input.Pos;
	output.Tex = input.Tex;
	output.Norm = mul(input.Norm, World);

	
	return output;
}

float4 PS(PS_INPUT_POS_NORM_TEX input) : SV_Target
{
	float ambient = 1.f;
	float4 finalColor = AlbedoTexture.Sample(linearSampling, input.Tex) * ambient;
	
	//finalColor.r = 1.f;
	//finalColor.g = 1.f;
	//finalColor.b = 1.f;
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