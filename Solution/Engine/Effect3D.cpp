#include "stdafx.h"

#include <d3dx11effect.h>
#include <D3DX11async.h>
#include "Effect3D.h"


namespace Easy3D
{
	Effect3D::Effect3D()
	{
	}


	Effect3D::~Effect3D()
	{
	}

	bool Effect3D::Init(const std::string& aEffectFile)
	{
		if (ReloadShader(aEffectFile) == false)
		{
			return false;
		}

		for (int i = 0; i < myEffectListeners.Size(); ++i)
		{
			myEffectListeners[i]->OnEffectLoad();
		}

		return true;
	}

	bool Effect3D::ReloadShader(const std::string& aFile)
	{
		Sleep(100);

		myFileName = aFile;

		HRESULT hr;
		unsigned int shaderFlags = 0;

#if defined (DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* compilationMsgs = nullptr;

		hr = D3DX11CompileFromFile(myFileName.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader
			, &compilationMsgs, 0);
		if (hr != S_OK)
		{
			if (compilationMsgs != nullptr)
			{
				DL_MESSAGE_BOX((char*)compilationMsgs->GetBufferPointer(), "Effect Error", MB_ICONWARNING);
				DL_ASSERT("Need to fix a dummy-shader 3D-Shader");
				//myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect(
				//	"Data/effect/SuperUglyDebugEffect.fx")->myEffect;
			}
		}
		if (compilationMsgs != nullptr)
		{
			compilationMsgs->Release();
		}

		if (hr == S_OK)
		{
			hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), NULL
				, Engine::GetInstance()->GetDevice(), &myEffect);

			if (FAILED(hr))
			{
				DL_MESSAGE_BOX("Cant Create Effect", "Effect Error", MB_ICONWARNING);
				return false;
			}

			compiledShader->Release();
		}


		myTechnique = myEffect->GetTechniqueByName("Render");
		if (myTechnique->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get Technique", "Effect Error", MB_ICONWARNING);
			return false;
		}

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