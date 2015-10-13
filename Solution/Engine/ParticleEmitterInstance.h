#pragma once

#include <GrowingArray.h>
#include <Matrix.h>
#include "ParticleInstance.h"

namespace Easy3D
{
	class Camera;
	class ParticleEmitterData;
	struct VertexBufferWrapper;

	class ParticleEmitterInstance
	{
	public:
		ParticleEmitterInstance(ParticleEmitterData& aEmitterData);
		~ParticleEmitterInstance();

		void Update(float aDelta);
		void Render(const Camera& aCamera);

		void PerformRotation(const CU::Matrix44<float>& aRotation);

		const CU::Matrix44<float>& GetOrientation() const;
		const CU::Vector3<float>& GetPosition() const;

		void SetOrientation(const CU::Matrix44<float>& aOrientation);
		void SetPosition(const CU::Vector3<float>& aPosition);

	private:
		void operator=(ParticleEmitterInstance&) = delete;

		void CreateVertexBuffer();

		void EmitterUpdate(float aDelta);
		void EmittParticle(float aDelta);
		void UpdateParticles(float aDelta);
		void UpdateVertexBuffer();

		float myGravity;
		float myProcessTime;

		CU::Matrix33<float> myEmissionOrientation;
		CU::Matrix44<float> myOrientation;
		CU::Vector3<float> myPosition;

		CU::GrowingArray<ParticleInstance> myParticles;
		CU::GrowingArray<int> myParticleIndices;

		ParticleEmitterData& myEmitterData;
		VertexBufferWrapper* myVertexBufferWrapper;
	};


	inline const CU::Matrix44<float>& ParticleEmitterInstance::GetOrientation() const
	{
		return myOrientation;
	}

	inline const CU::Vector3<float>& ParticleEmitterInstance::GetPosition() const
	{
		return myPosition;
	}
	 
	inline void ParticleEmitterInstance::SetOrientation(const CU::Matrix44<float>& aOrientation)
	{
		myOrientation = aOrientation;
	}

	inline void ParticleEmitterInstance::SetPosition(const CU::Vector3<float>& aPosition)
	{
		myPosition = aPosition;
		myOrientation.SetPos(myPosition);
	}
}

