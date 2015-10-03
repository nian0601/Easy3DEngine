#pragma once

#include "BaseEffect.h"
#include "Defines.h"
#include "LightStructs.h"
#include <Matrix.h>
#include <string.h>
#include <StaticArray.h>

struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectVectorVariable;

namespace Easy3D
{
	class Effect3D : public BaseEffect
	{
	public:
		bool Init(const std::string& aEffectFile) override;
		bool ReloadShader(const std::string& aFile) override;

		void SetScaleVector(const CU::Vector3<float>& aScaleVector);
		void SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix);
		void SetViewMatrix(const CU::Matrix44<float>& aViewMatrix);
		void SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix);

		void UpdateDirectionalLights(
			const CU::StaticArray<DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData);
		void UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData);
		void UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData);

	private:

		ID3DX11EffectMatrixVariable* myProjectionMatrixVariable;
		ID3DX11EffectMatrixVariable* myViewMatrixVariable;
		ID3DX11EffectMatrixVariable* myWorldMatrixVariable;

		ID3DX11EffectVectorVariable* myScaleVectorVariable;

		ID3DX11EffectVariable* myDirectionalLightVariable;
		ID3DX11EffectVariable* myPointLightVariable;
		ID3DX11EffectVariable* mySpotLightVariable;
	};
}