#include "stdafx.h"

#include <d3dx11effect.h>
#include "Effect.h"
#include "Surface.h"
#include "Texture.h"
#include "TextureContainer.h"

Prism::Surface::Surface()
{
	myTextures.Init(2);
	myShaderViews.Init(2);
	myFilePaths.Init(2);
	myShaderResourceNames.Init(2);
}

bool Prism::Surface::SetTexture(const std::string& aResourceName, const std::string& aFileName, bool aUseSRGB)
{
	aUseSRGB;

	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(aFileName);
	ID3DX11EffectShaderResourceVariable* shaderVar = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();

	if (shaderVar->IsValid() == false)
	{
		std::string errorMsg = "Failed to get ShaderResource: " + aResourceName;
		//DL_MESSAGE_BOX(errorMsg.c_str(), "Surface Error", MB_ICONWARNING);
		RESOURCE_LOG(errorMsg.c_str());
		return false;
	}

	myTextures.Add(tex);
	myShaderViews.Add(shaderVar);
	myFilePaths.Add(aFileName);
	myShaderResourceNames.Add(aResourceName);

	return true;
}

void Prism::Surface::ReloadSurface()
{
	myTextures.RemoveAll();
	myShaderViews.RemoveAll();

	for (int i = 0; i < myFilePaths.Size(); ++i)
	{
		Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(myFilePaths[i]);
		ID3DX11EffectShaderResourceVariable* shaderVar = myEffect->GetEffect()->GetVariableByName(myShaderResourceNames[i].c_str())->AsShaderResource();

		if (shaderVar->IsValid() == false)
		{
			std::string errorMsg = "Failed to get ShaderResource: " + myShaderResourceNames[i];
			//DL_MESSAGE_BOX(errorMsg.c_str(), "Surface Error", MB_ICONWARNING);
			RESOURCE_LOG(errorMsg.c_str());
		}

		myTextures.Add(tex);
		myShaderViews.Add(shaderVar);
	}
}

bool Prism::Surface::SetTexture(const std::string& aResourceName, Texture* aTexture)
{
	ID3DX11EffectShaderResourceVariable* shaderVar = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();
	if (shaderVar->IsValid() == false)
	{
		//DL_MESSAGE_BOX("Failed to get ShaderResource", "Surface Error", MB_ICONWARNING);
		RESOURCE_LOG("Failed to get ShaderResource");
		return false;
	}

	myTextures.Add(aTexture);
	myShaderViews.Add(shaderVar);

	return true;
}

void Prism::Surface::Activate()
{
	Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(myPrimitiveTopologyType);

	for (int i = 0; i < myShaderViews.Size(); ++i)
	{
		myShaderViews[i]->SetResource(myTextures[i]->GetShaderView());
	}
}