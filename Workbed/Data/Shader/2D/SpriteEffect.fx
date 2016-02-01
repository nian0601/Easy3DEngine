#include "Shared2D.fx"

PS_INPUT_POS_TEX VS(VS_INPUT_POS_TEX input)
{
	PS_INPUT_POS_TEX output = (PS_INPUT_POS_TEX)0;
	
	float4 pos = input.Pos;
	pos.x *= PosAndScale.z;
	pos.y *= PosAndScale.w;
	pos.x += PosAndScale.x;
	pos.y += PosAndScale.y;
	output.Pos = mul(pos, Projection);
	output.Pos.x -= 1;
	output.Pos.y += 1;
	output.Tex = input.Tex;

	return output;
}

float4 PS(PS_INPUT_POS_TEX input) : SV_Target
{
	float4 color = Texture.Sample(linearSampling, input.Tex);

	float pixelAlpha = color.a;

	color.r *= Color.r * pixelAlpha;
	color.g *= Color.g * pixelAlpha;
	color.b *= Color.b * pixelAlpha;

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