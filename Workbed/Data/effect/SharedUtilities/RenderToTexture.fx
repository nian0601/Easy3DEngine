#include "ShaderVariables.fx"


SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}
float4 PS(PS_INPUT_POS_TEX input) : SV_Target
{
	return DiffuseTexture.Sample(linearSampling, input.Tex);
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