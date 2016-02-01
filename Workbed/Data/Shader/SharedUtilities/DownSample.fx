#include "ShaderVariables.fx"

SamplerState bilinearSampling
{
	Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
};

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}

float LinearToLum(float4 aColor)
{
	return(aColor.r*0.3+aColor.g*0.59+aColor.b*0.11);
}

float4 PS_BLOOM(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = DiffuseTexture.SampleLevel(bilinearSampling, input.Tex, 0);

	if(LinearToLum(color) > 0.1)
	{
		return color;
	}

	return float4(0, 0, 0, 1);
}

float4 PS_HDR(PS_INPUT_POS_TEX input) : SV_Target
{
	return DiffuseTexture.SampleLevel(bilinearSampling, input.Tex, 0);
}

technique11 BLOOM_DOWNSAMPLE
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_BLOOM()));
	}
}

technique11 HDR_DOWNSAMPLE
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_HDR()));
	}
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(NULL);
		SetGeometryShader(NULL);
		SetPixelShader(NULL);
	}
}