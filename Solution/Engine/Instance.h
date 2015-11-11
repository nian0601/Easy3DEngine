#pragma once

#include "Defines.h"
#include "LightStructs.h"
#include <Matrix.h>
#include <StaticArray.h>
#include "TransformationNodeInstance.h"

namespace Easy3D
{
	class Camera;
	class Light;
	class ModelProxy;
	class RenderProcessTarget;

	class Instance
	{
	public:
		Instance(ModelProxy& aModel, const CU::Matrix44<float>& aOrientation);
		~Instance();

		void BuildHierarchy(TransformationNodeInstance& aHierarchy, Model* aModel);

		void Update(float aDelta);

		void Render(Camera& aCamera);
		void Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera);
		void RenderModel(Model* aModel, const CU::Matrix44<float>& aParent, Camera& aCamera
			, TransformationNodeInstance& aHierarchy);
		void AddRenderData(RenderProcessTarget* aTarget);

		void SetEffect(const std::string& aEffectFile);

		void SetScale(const CU::Vector3<float>& aScaleVector);

		void UpdateDirectionalLights(
			const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData);
		void UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData);
		void UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData);

	private:
		void operator=(Instance&) = delete;

		ModelProxy& myProxy;
		const CU::Matrix44<float>& myOrientation;
		CU::Vector3<float> myScale;
		CU::Vector3<float> myPosition;

		TransformationNodeInstance myHierarchy;
	};
}