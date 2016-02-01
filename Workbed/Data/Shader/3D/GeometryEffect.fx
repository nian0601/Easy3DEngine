#include "../SharedUtilities/ShaderVariables.fx"

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
};

PS_INPUT_POS_NORM_COL VS(VS_INPUT_POS_NORM_COL input)
{
	PS_INPUT_POS_NORM_COL output = (PS_INPUT_POS_NORM_COL)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = input.Color;
	output.Norm = mul(input.Norm, World);
	
	return output;
}

float4 PS(PS_INPUT_POS_NORM_COL input) : SV_Target
{
	float ambient = 0.1f;
	float4 finalColor = input.Color * ambient;
	
	
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