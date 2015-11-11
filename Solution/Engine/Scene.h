#pragma once
#include "Defines.h"
#include <GrowingArray.h>
#include "LightStructs.h"
#include <StaticArray.h>



namespace Easy3D
{
	class Camera;
	class DirectionalLight;
	class Instance;
	class ParticleEmitterInstance;
	class PointLight;
	class SpotLight;
	class RenderProcessTarget;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void Update(float aDelta);

		void Render();
		void Render(RenderProcessTarget* aTarget);

		void AddInstance(Instance* aInstance);
		void AddInstance(ParticleEmitterInstance* aInstance);
		void AddLight(DirectionalLight* aLight);
		void AddLight(PointLight* aLight);
		void AddLight(SpotLight* aLight);

		void RemoveInstance(Instance* aInstance);

		void SetCamera(Camera* aCamera);
		Camera& GetCamera();

	private:
		CU::GrowingArray<Instance*> myInstances;
		CU::GrowingArray<ParticleEmitterInstance*> myEmitterInstances;
		CU::GrowingArray<DirectionalLight*> myDirectionalLights;
		CU::GrowingArray<PointLight*> myPointLights;
		CU::GrowingArray<SpotLight*> mySpotLights;

		Camera* myCamera;

		CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS> myDirectionalLightData;
		CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS> myPointLightData;
		CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS> mySpotLightData;
	};
}