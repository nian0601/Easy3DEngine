#include "ShaderVariables.fx"


SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

float TexelWidth;
float TexelHeight;


PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;

	output.Tex = input.Tex;
	output.Pos = input.Pos;

	return output;
}

static const int kernelSize = 13;
float PixelKernel[kernelSize] =
{
	-6,
	-5,
	-4,
	-3,
	-2,
	-1,
	0,
	+1,
	+2,
	+3,
	+4,
	+5,
	+6
};
static const float BlurWeights[kernelSize] =
{
	0.002216,
	0.008764,
	0.026995,
	0.064759,
	0.120985,
	0.176033,
	0.199471,
	0.176033,
	0.120985,
	0.064759,
	0.026995,
	0.008764,
	0.002216,
};


float4 PS_X(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = {0, 0, 0, 1};
	return color;
	float3 uv;
	uv.y = input.Tex.y;

	for(int i = 0; i < kernelSize; ++i)
	{
		uv.x = input.Tex.x + (PixelKernel[i] * TexelWidth);
		color += DiffuseTexture.SampleLevel(linearSampling, uv, 0) * BlurWeights[i];
	}

	return color;	
}

float4 PS_Y(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = {0, 0, 0, 1};
	return color;
	float3 uv;
	uv.x = input.Tex.x;

	for(int i = 0; i < kernelSize; ++i)
	{
		uv.y = input.Tex.y + (PixelKernel[i] * TexelWidth);
		color += DiffuseTexture.SampleLevel(linearSampling, uv, 0) * BlurWeights[i];
	}

	return color;	
}


technique11 BLOOM_X
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_X()));
	}
}

technique11 BLOOM_Y
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Y()));
	}
}


technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_X()));
	}
}