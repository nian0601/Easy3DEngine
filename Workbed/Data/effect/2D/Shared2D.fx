Matrix Projection;
float4 PosAndScale;
float4 Color;
Texture2D Texture;

SamplerState linearSampling
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
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