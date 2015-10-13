#pragma once
#include <Vector.h>

namespace Easy3D
{
	struct ParticleData
	{
		bool myParticleFollowEmitter = false;
		bool myGravityFlag = false;
		float myStartAlpha = 1.f;
		float myAlphaDelta = -0.5f;
		float myLifeTime = 10.f;
		float myMinStartSize = 0.1f;
		float myMaxStartSize = 1.f;
		CU::Vector3<float> myMinVelocity = { 0.1f, 0.1f, 0.1f };
		CU::Vector3<float> myMaxVelocity = { 1.f, 1.f, 1.f };
		float mySizeDelta = 0.f;
	};
}