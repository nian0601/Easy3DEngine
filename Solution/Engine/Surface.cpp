#include "stdafx.h"

#include "BaseEffect.h"
#include <d3dx11effect.h>
#include "Surface.h"
#include "Texture.h"
#include "TextureContainer.h"

Easy3D::Surface::Surface()
{
	myTextures.Init(2);
	myShaderVariables.Init(2);
	myFilePaths.Init(2);
	myShaderResourceNames.Init(2);
	myShaderResoures.Init(2);
}

bool Easy3D::Surface::SetTexture(const std::string& aResourceName, const std::string& aFileName, bool aUseSRGB)
{
	aUseSRGB;

	Texture* tex = TextureContainer::GetInstance()->GetTexture(aFileName);
	ID3DX11EffectShaderResourceVariable* shaderVar = nullptr;
	if (myEffect != nullptr)
	{
		shaderVar = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();
	}
	myTextures.Add(tex);
	myShaderVariables.Add(shaderVar);
	myFilePaths.Add(aFileName);
	myShaderResourceNames.Add(aResourceName);
	myShaderResoures.Add(tex->GetShaderView());

	return true;
}

void Easy3D::Surface::ReloadSurface()
{
	myTextures.RemoveAll();
	myShaderVariables.RemoveAll();

	for (int i = 0; i < myFilePaths.Size(); ++i)
	{
		Texture* tex = TextureContainer::GetInstance()->GetTexture(myFilePaths[i]);
		ID3DX11EffectShaderResourceVariable* shaderVar = nullptr;
		if (myEffect != nullptr)
		{
			shaderVar = myEffect->GetEffect()->GetVariableByName(myShaderResourceNames[i].c_str())->AsShaderResource();
		}

		if (shaderVar->IsValid() == false)
		{
			std::string errorMsg = "Failed to get ShaderResource: " + myShaderResourceNames[i];
			//DL_MESSAGE_BOX(errorMsg.c_str(), "Surface Error", MB_ICONWARNING);
			RESOURCE_LOG(errorMsg.c_str());
		}

		myTextures.Add(tex);
		myShaderVariables.Add(shaderVar);
		myShaderResoures.Add(tex->GetShaderView());
	}
}

void Easy3D::Surface::SetAlbedoResource(ID3D11ShaderResourceView* aResource)
{
	myShaderResoures[0] = aResource;
}

bool Easy3D::Surface::SetTexture(const std::string& aResourceName, Texture* aTexture)
{
	ID3DX11EffectShaderResourceVariable* shaderVar = nullptr;
	if (myEffect != nullptr)
	{
		shaderVar = myEffect->GetEffect()->GetVariableByName(aResourceName.c_str())->AsShaderResource();
	}
	
	if (shaderVar->IsValid() == false)
	{
		//DL_MESSAGE_BOX("Failed to get ShaderResource", "Surface Error", MB_ICONWARNING);
		RESOURCE_LOG("Failed to get ShaderResource");
		return false;
	}

	myTextures.Add(aTexture);
	myShaderVariables.Add(shaderVar);
	myShaderResoures.Add(aTexture->GetShaderView());

	return true;
}

void Easy3D::Surface::Activate()
{
	Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(myPrimitiveTopologyType);

	for (int i = 0; i < myShaderVariables.Size(); ++i)
	{
		myShaderVariables[i]->SetResource(myShaderResoures[i]);
	}
}