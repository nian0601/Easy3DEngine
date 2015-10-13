#pragma once
#include <Vector.h>
#include <string>
#include "ParticleData.h"

struct ID3D11InputLayout;

namespace Easy3D
{
	class ParticleEffect;
	class Texture;

	class ParticleEmitterData
	{
		friend class ParticleEmitterInstance;

	public:
		void Init(const std::string& aFile);

	private:
		void LoadDataFile(const std::string& aFile);

		ParticleEffect* myEffect;
		float myEmissionLifeTime;
		float myEmissionRate;
		float myEmissionRateDelta;
		float myEmissionSpeed;
		float myGravity;
		int myMaxNrOfParticles;
		CU::Vector3<float> myEmissionExtents;
		CU::Vector3<float> myEmissionVelocityDelta;
		CU::Vector3<float> myMaxVelocity;
		CU::Vector3<float> myMinVelocity;
		ParticleData myParticleData;
		ID3D11InputLayout* myParticleInputLayout;
		Texture* myTexture;
		bool myHasNewData;
	};
}

