#include "stdafx.h"

#include <d3dx11effect.h>
#include <D3DX11async.h>
#include "Effect3D.h"


namespace Easy3D
{
	bool Effect3D::Init(const std::string& aEffectFile)
	{
		return BaseEffect::Init(aEffectFile);
	}

	bool Effect3D::ReloadShader(const std::string& aFile)
	{
		BaseEffect::ReloadShader(aFile);


		myScaleVectorVariable = myEffect->GetVariableByName("Scale")->AsVector();
		if (myScaleVectorVariable->IsValid() == false)
		{
			myScaleVectorVariable = nullptr;
		}

		myWorldMatrixVariable = myEffect->GetVariableByName("World")->AsMatrix();
		if (myWorldMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get WorldMatrix", "Effect Error", MB_ICONWARNING);
			return false;
		}

		myViewMatrixVariable = myEffect->GetVariableByName("View")->AsMatrix();
		if (myViewMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ViewMatrix", "Effect Error", MB_ICONWARNING);
			return false;
		}

		myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
		if (myProjectionMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "Effect Error", MB_ICONWARNING);
			return false;
		}

		myDirectionalLightVariable = myEffect->GetVariableByName("DirectionalLights");
		if (myDirectionalLightVariable->IsValid() == false)
		{
			myDirectionalLightVariable = nullptr;
		}

		myPointLightVariable = myEffect->GetVariableByName("PointLights");
		if (myPointLightVariable->IsValid() == false)
		{
			myPointLightVariable = nullptr;
		}

		mySpotLightVariable = myEffect->GetVariableByName("SpotLights");
		if (mySpotLightVariable->IsValid() == false)
		{
			mySpotLightVariable = nullptr;
		}

		return true;
	}

	void Effect3D::SetScaleVector(const CU::Vector3<float>& aScaleVector)
	{
		if (myScaleVectorVariable != nullptr)
		{
			myScaleVectorVariable->SetFloatVector(&aScaleVector.x);
		}
		
	}

	void Effect3D::SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix)
	{
		myWorldMatrixVariable->SetMatrix(&aWorldMatrix.myMatrix[0]);
	}

	void Effect3D::SetViewMatrix(const CU::Matrix44<float>& aViewMatrix)
	{
		myViewMatrixVariable->SetMatrix(&aViewMatrix.myMatrix[0]);
	}

	void Effect3D::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
	{
		myProjectionMatrixVariable->SetMatrix(&aProjectionMatrix.myMatrix[0]);
	}

	void Effect3D::UpdateDirectionalLights(
		const CU::StaticArray<Easy3D::DirectionalLightData, NUMBER_OF_DIRECTIONAL_LIGHTS>& someDirectionalLightData)
	{
		if (myDirectionalLightVariable != nullptr)
		{
			myDirectionalLightVariable->SetRawValue(&someDirectionalLightData[0], 0,
				sizeof(DirectionalLightData) * NUMBER_OF_DIRECTIONAL_LIGHTS);
		}
	}

	void Effect3D::UpdatePointLights(const CU::StaticArray<PointLightData, NUMBER_OF_POINT_LIGHTS>& somePointLightData)
	{
		if (myPointLightVariable != nullptr)
		{
			myPointLightVariable->SetRawValue(&somePointLightData[0], 0, sizeof(PointLightData) * NUMBER_OF_POINT_LIGHTS);
		}
	}

	void Effect3D::UpdateSpotLights(const CU::StaticArray<SpotLightData, NUMBER_OF_SPOT_LIGHTS>& someSpotLightData)
	{
		if (mySpotLightVariable != nullptr)
		{
			mySpotLightVariable->SetRawValue(&someSpotLightData[0], 0, sizeof(SpotLightData) * NUMBER_OF_SPOT_LIGHTS);
		}
	}
}