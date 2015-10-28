struct DIRECTIONAL_LIGHT
{
	float4 Direction;
	float4 Color;
};

DIRECTIONAL_LIGHT DirectionalLights[2];


float4 CalculateDirectionalLight(DIRECTIONAL_LIGHT aLight, float3 aNormal)
{
	float lambert = dot((float3)aLight.Direction, aNormal);
	float4 lightColor = saturate(lambert) * aLight.Color;

	return lightColor;
}