#include "../SharedUtilities/DirectionalLights.fx"
#include "../SharedUtilities/ShaderVariables.fx"
#include "../SharedUtilities/PointLights.fx"
#include "../SharedUtilities/SpotLights.fx"

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

GS_LINE VS(VS_LINE input)
{
	GS_LINE output = (GS_LINE)0;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Color = input.Color;

	return output;
}

float4 PS(PS_LINE input) : SV_Target
{
	return input.Color;
}

[maxvertexcount(6)]
void GS(line GS_LINE input[2], inout TriangleStream<PS_LINE> triStream)
{
	float4 p0 = input[0].Position;
    float4 p1 = input[1].Position;

    float w0 = p0.w;
    float w1 = p1.w;

    p0.xyz /= p0.w;
    p1.xyz /= p1.w;

    float3 line01 = p1 - p0;
    float3 dir = normalize(line01);

    float3 unit_z = normalize(float3(0, 0, -1));

    float3 normal = normalize(cross(unit_z, dir));

    float width = 0.01;

    PS_LINE v[4];

    float3 dir_offset = dir *  width;
    float3 normal_scaled = normal *  width;

    float3 p0_ex = p0 - dir_offset;
    float3 p1_ex = p1 + dir_offset;

    v[0].Position = float4(p0_ex - normal_scaled, 1) * w0;
    v[0].Color = input[0].Color;

    v[1].Position = float4(p0_ex + normal_scaled, 1) * w0;
    v[1].Color = input[0].Color;

    v[2].Position = float4(p1_ex + normal_scaled, 1) * w1;
    v[2].Color = input[1].Color;

    v[3].Position = float4(p1_ex - normal_scaled, 1) * w1;
    v[3].Color = input[1].Color;

    triStream.Append(v[2]);
    triStream.Append(v[1]);
    triStream.Append(v[0]);

    triStream.RestartStrip();

    triStream.Append(v[3]);
    triStream.Append(v[2]);
    triStream.Append(v[0]);

    triStream.RestartStrip();
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