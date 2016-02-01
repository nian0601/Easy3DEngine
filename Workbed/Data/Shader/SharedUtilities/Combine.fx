#include "ShaderVariables.fx"

Texture2D SourceA;
Texture2D SourceB;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
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
	float4 color = SourceA.SampleLevel(linearSampling, input.Tex, 0);
	color += SourceB.SampleLevel(linearSampling, input.Tex, 0);
	return color;
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