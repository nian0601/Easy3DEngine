#include "stdafx.h"

#include <d3dx11effect.h>
#include <D3DX11async.h>
#include "ParticleEffect.h"


namespace Easy3D
{
	bool ParticleEffect::Init(const std::string& aEffectFile)
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

	bool ParticleEffect::ReloadShader(const std::string& aFile)
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
				DL_ASSERT("Need to fix a dummy-shader for Particles");
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
				DL_MESSAGE_BOX("Cant Create Effect", "ParticleEffect Error", MB_ICONWARNING);
				return false;
			}

			compiledShader->Release();
		}


		myTechnique = myEffect->GetTechniqueByName("Render");
		if (myTechnique->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get Technique", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}


		myWorldMatrixVariable = myEffect->GetVariableByName("World")->AsMatrix();
		if (myWorldMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get WorldMatrix", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		myViewMatrixVariable = myEffect->GetVariableByName("View")->AsMatrix();
		if (myViewMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ViewMatrix", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
		if (myProjectionMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		
		myAlphaDeltaVariable = myEffect->GetVariableByName("AlphaDelta")->AsScalar();
		if (myAlphaDeltaVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get AlphaDelta", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		mySizeDeltaVariable = myEffect->GetVariableByName("SizeDelta")->AsScalar();
		if (mySizeDeltaVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get SizeDelta", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		myStartAlphaVariable = myEffect->GetVariableByName("StartAlpha")->AsScalar();
		if (myStartAlphaVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get StartAlpha", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		myTextureVariable = myEffect->GetVariableByName("ParticleTexture")->AsShaderResource();
		if (myTextureVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ParticleTexture", "ParticleEffect Error", MB_ICONWARNING);
			return false;
		}

		return true;
	}

	void ParticleEffect::SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix)
	{
		myWorldMatrixVariable->SetMatrix(&aWorldMatrix.myMatrix[0]);
	}

	void ParticleEffect::SetViewMatrix(const CU::Matrix44<float>& aViewMatrix)
	{
		myViewMatrixVariable->SetMatrix(&aViewMatrix.myMatrix[0]);
	}

	void ParticleEffect::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
	{
		myProjectionMatrixVariable->SetMatrix(&aProjectionMatrix.myMatrix[0]);
	}

	void ParticleEffect::SetAlphaDelta(float aDelta)
	{
		myAlphaDeltaVariable->SetFloat(aDelta);
	}

	void ParticleEffect::SetSizeDelta(float aDelta)
	{
		mySizeDeltaVariable->SetFloat(aDelta);
	}

	void ParticleEffect::SetStartAlpha(float aStartAlpha)
	{
		myStartAlphaVariable->SetFloat(aStartAlpha);
	}

	void ParticleEffect::SetTexture(Texture* aTexture)
	{
		myTextureVariable->SetResource(aTexture->GetShaderView());
	}

}
