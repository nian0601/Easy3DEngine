#pragma once

namespace Easy3D
{
	enum class eDepthStencil
	{
		Z_ENABLED,
		Z_DISABLED,
		PARTICLES,
	};

	enum class eRasterizer
	{
		CULL_FRONT,
		WIRE_FRAME,
		NO_CULLING,
	};

	enum class eBlendState
	{
		ALPHA,
		NO_ALPHA
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