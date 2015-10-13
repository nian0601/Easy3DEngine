#include "stdafx.h"

#include <d3dx11effect.h>
#include <D3DX11async.h>
#include "Effect2D.h"

namespace Easy3D
{
	bool Effect2D::Init(const std::string& aEffectFile)
	{
		myPosAndScale = CU::Vector4<float>(0.f, 0.f, 1.f, 1.f);

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

	bool Effect2D::ReloadShader(const std::string& aFile)
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


		myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
		if (myProjectionMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "Effect Error", MB_ICONWARNING);
			return false;
		}

		myPosAndScaleVariable = myEffect->GetVariableByName("PosAndScale")->AsVector();
		if (myPosAndScaleVariable->IsValid() == false)
		{
			myPosAndScaleVariable = nullptr;
		}

		myColorVariable = myEffect->GetVariableByName("Color")->AsVector();
		if (myColorVariable->IsValid() == false)
		{
			myColorVariable = nullptr;
		}

		return true;
	}
		

	void Effect2D::UpdatePosAndScale(const CU::Vector2<float>& aPos, const CU::Vector2<float>& aScale)
	{
		myPosAndScale.x = aPos.x;
		myPosAndScale.y = aPos.y;

		myPosAndScale.z = aScale.x;
		myPosAndScale.w = aScale.y;
	}

	void Effect2D::SetPosAndScale()
	{
		DL_ASSERT_EXP(myPosAndScaleVariable != nullptr
			, "Effect2D: Tried to SetPosAndScale but myPosAndScaleVariable is nullptr");

		myPosAndScaleVariable->SetFloatVector(&myPosAndScale.x);
	}

	void Effect2D::SetColor(const CU::Vector4<float>& aColor)
	{
		DL_ASSERT_EXP(myColorVariable != nullptr
			, "Effect2D: Tried to SetColor but myColorVariable is nullptr");

		myColorVariable->SetFloatVector(&aColor.x);
	}

	void Effect2D::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
	{
		DL_ASSERT_EXP(myProjectionMatrixVariable != nullptr,
			"Effect2D: Tried to SetProjectionMatrix but myProjectionMatrixVariable is nullptr");

		myProjectionMatrixVariable->SetMatrix(&aProjectionMatrix.myMatrix[0]);
	}
}