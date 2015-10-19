#include "stdafx.h"
#include "BaseEffect.h"
#include <d3dx11effect.h>
#include <D3DX11async.h>

namespace Easy3D
{
	BaseEffect::BaseEffect()
		: myEffect(nullptr)
		, myTechnique(nullptr)
		, myEffectListeners(4)
	{
	}

	BaseEffect::~BaseEffect()
	{
	}

	bool BaseEffect::Init(const std::string& aEffectFile)
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

	void BaseEffect::SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask)
	{
		Engine::GetInstance()->GetContex()->OMSetBlendState(aBlendState, aBlendFactor, aSampleMask);
	}

	bool BaseEffect::ReloadShader(const std::string& aFile)
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
				DL_ASSERT("Need to fix a dummy-shader");
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

		return true;
	}

}