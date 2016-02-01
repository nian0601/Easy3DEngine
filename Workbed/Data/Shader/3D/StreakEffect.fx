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
void GS(line GS_INPUT input[2], inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT vertex = (PS_INPUT)0;

	float4 lineDir = input[1].Position - input[0].Position;
	lineDir = normalize(lineDir);

	float3 toEye = float3(View._41, View._42, View._43) - input[0].Position.xyz;
	normalize(toEye);
	float3 normal = cross(toEye, lineDir.xyz);
	normalize(normal);

	float width = input[0].Size;
	float uvMult = 5;
	float uvSegment = 1;
	PS_INPUT output[4];// = (PS_INPUT)0;

	output[0].Position = float4(input[0].Position.xyz - normal * width, 1);
	output[0].Position = mul(output[0].Position, Projection);
	output[0].TexCoord = float2(input[0].Time * uvMult, 1);
	output[0].Alpha = input[0].Alpha;

	output[1].Position = float4(input[1].Position.xyz - normal * width, 1);
	output[1].Position = mul(output[1].Position, Projection);
	output[1].TexCoord = float2(input[0].Time * uvMult + uvSegment, 1);
	output[1].Alpha = input[1].Alpha;

	output[2].Position = float4(input[0].Position.xyz + normal * width, 1);
	output[2].Position = mul(output[2].Position, Projection);
	output[2].TexCoord = float2(input[0].Time * uvMult, 0);
	output[2].Alpha = input[0].Alpha;

	output[3].Position = float4(input[1].Position.xyz + normal * width, 1);
	output[3].Position = mul(output[3].Position, Projection);
	output[3].TexCoord = float2(input[0].Time * uvMult + uvSegment, 0);
	output[3].Alpha = input[1].Alpha;


	triStream.Append(output[0]);
	triStream.Append(output[1]);
	triStream.Append(output[2]);
	triStream.Append(output[3]);

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