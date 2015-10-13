#pragma once
#include "Vector.h"

namespace Easy3D
{
	struct ParticleInstance
	{
		CU::Vector3<float> myPosition;
		CU::Vector3<float> myVelocity;
		float mySize;
		float myAlpha;
		float myTime;
	};
}

