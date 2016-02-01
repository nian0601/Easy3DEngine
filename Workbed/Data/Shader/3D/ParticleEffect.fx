struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Velocity : VELOCITY;
	float Size : SIZE;
	float Alpha : ALPHA;
	float Time : TIME;
};

struct GS_INPUT
{
	float4 Position : SV_POSITION;
	float3 Velocity : VELOCITY;
	float Size : SIZE;
	float Alpha : ALPHA;
	float Time : TIME;
};

struct PS_INPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float Alpha : ALPHA;
};

Texture2D ParticleTexture;
Matrix World;
Matrix View;
Matrix Projection;
float AlphaDelta;
float SizeDelta;
float StartAlpha;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

GS_INPUT VS(VS_INPUT input)
{
	GS_INPUT output = (GS_INPUT)0;

	output.Position = float4(input.Position.xyz, 1);
	output.Position = mul(output.Position, World);
	output.Position = mul(output.Position, View);
	output.Velocity = input.Velocity;
	output.Size = input.Size;
	output.Alpha = input.Alpha;
	output.Time = input.Time;

	return output;
}

[maxvertexcount(4)]
void GS(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT vertex = (PS_INPUT)0;

	const float4 offsets[4] =
	{
		{-input[0].Size, input[0].Size, 0, 0},
		{input[0].Size, input[0].Size, 0, 0},
		{-input[0].Size, -input[0].Size, 0, 0},
		{input[0].Size, -input[0].Size, 0, 0}
	};

	const float2 uv_coords[4] =
	{
		{0, 1},
		{1, 1},
		{0, 0},
		{1, 0}
	};

	for(int i = 0; i < 4; ++i)
	{
		vertex.Position = input[0].Position + offsets[i];
		vertex.Position = mul(vertex.Position, Projection);
		vertex.TexCoord = uv_coords[i];
		vertex.Alpha = input[0].Alpha;
		triStream.Append(vertex);
	}

	triStream.RestartStrip();
}

float4 PS(PS_INPUT input) : SV_Target
{
	float4 color = ParticleTexture.Sample(linearSampling, input.TexCoord);
	color.w = color.w * input.Alpha;
	return color;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}