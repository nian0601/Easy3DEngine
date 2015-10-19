#pragma once

namespace Easy3D
{
	enum class eDepthStencilType
	{
		Z_ENABLED,
		Z_DISABLED,
		PARTICLES,
	};

	enum ePostProcess
	{
		BLOOM = 1,
		MOTION_BLUR = 2,
	};

	enum eRendererTextures
	{
		SCENE_TEXTURE,
		VELOCITY_TEXTURE,
		FINAL_TEXTURE,
		_SIZE
	};
}