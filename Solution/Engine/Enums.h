#pragma once

namespace Easy3D
{
	enum class eDepthStencilType
	{
		Z_ENABLED,
		Z_DISABLED,
		PARTICLES,
	};

	enum class eRasterizerType
	{
		CULL_FRONT,
		WIRE_FRAME,
		NO_CULLING,
	};

	enum ePostProcess
	{
		NONE = 0,
		HDR = 1,
		BLOOM = 2,
		MOTION_BLUR = 4,
	};

	enum eRendererTextures
	{
		SCENE_TEXTURE,
		VELOCITY_TEXTURE,
		FINAL_TEXTURE,
		_SIZE
	};
}