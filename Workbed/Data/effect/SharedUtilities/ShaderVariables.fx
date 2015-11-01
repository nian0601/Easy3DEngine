Matrix World;
Matrix View;
Matrix Projection;

Texture2D DiffuseTexture;

Texture2D AlbedoTexture;
Texture2D NormalTexture;
Texture2D RoughnessTexture;
Texture2D MetalnessTexture;
Texture2D AOTexture;

TextureCube CubeMap;

float3 Scale;
float4 ColorMask;

struct VS_INPUT_POS_NORM_TEX
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_NORM_TEX
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};



struct VS_INPUT_POS_NORM_TEX_BI_TANG
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
};

struct PS_INPUT_POS_NORM_TEX_BI_TANG
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float3 BiNormal : BINORMAL;
	float3 Tangent : TANGENT;
	float4 WorldPosition : POSITION;
};



struct VS_INPUT_POS_TEX
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_TEX
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};



struct VS_INPUT_POS_COL
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT_POS_COL
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};



struct VS_INPUT_POS_NORM_COL
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};

struct PS_INPUT_POS_NORM_COL
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float4 Color : COLOR;
};



struct VS_INPUT_POS_COL_UV
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT_POS_COL_UV
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};