#pragma once

#include "Defines.h"
#include "LightStructs.h"
#include <Matrix.h>
#include <StaticArray.h>

namespace Easy3D
{
	class Camera;
	class Light;
	class ModelProxy;

	class Instance
	{
	public:
		Instance(ModelProxy& aModel, const CU::Matrix44<float>& aOrientation, const bool& aIsActive);
		~Instance();

		void Render(Camera& aCamera);
		void Render(const CU::Matrix44<float>& aParentMatrix, Camera& aCamera);

		void SetEffect(const std::string& aEffectFile);

		void SetScale(const CU::Vector3<float>& aScaleVector);
		void SetColor(const CU::Vector4<float>& aColor);

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
		CU::Vector4<float> myColor;
		const bool& myIsActive;
	};
}