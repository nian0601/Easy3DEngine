#include "stdafx.h"

#include <d3dx11effect.h>
#include "Engine.h"
#include "Effect3D.h"
#include "EffectContainer.h"
#include "FileWatcher.h"
#include "Texture.h"
#include "TextureContainer.h"

Easy3D::Effect3D* Easy3D::EffectContainer::Get3DEffect(const std::string& aFilePath)
{
	auto it = my3DEffects.find(aFilePath);

	if (it == my3DEffects.end())
	{
		Load3DEffect(aFilePath);
	}

	return my3DEffects[aFilePath];
}

void Easy3D::EffectContainer::Load3DEffect(const std::string& aFilePath)
{
	Effect3D* newEffect = new Effect3D();

	if (newEffect->Init(aFilePath) == false)
	{
		std::stringstream ss;
		ss << "Failed to Init Effect: " << aFilePath.c_str() << std::endl;
		DL_MESSAGE_BOX(ss.str().c_str(), "EffectContainer::LoadEffect", MB_ICONWARNING);
		return;
	}

	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(myCubeMap);
	ID3DX11EffectShaderResourceVariable* shaderVar = newEffect->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

	if (shaderVar->IsValid())
	{
		shaderVar->SetResource(tex->GetShaderView());
	}


	DL_ASSERT_EXP(newEffect != nullptr, "newEffect is nullpter in LoadEffect, HOW?");

	my3DEffects[aFilePath] = newEffect;

	WATCH_FILE(aFilePath, Easy3D::EffectContainer::Reload3DEffect);
}

void Easy3D::EffectContainer::Reload3DEffect(const std::string& aFilePath)
{
	if (my3DEffects.find(aFilePath) == my3DEffects.end())
	{
		return;
	}

	my3DEffects[aFilePath]->Init(aFilePath);


	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(myCubeMap);
	ID3DX11EffectShaderResourceVariable* shaderVar = my3DEffects[aFilePath]->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

	if (shaderVar->IsValid())
	{
		shaderVar->SetResource(tex->GetShaderView());
	}
}

void Easy3D::EffectContainer::SetCubeMap(const std::string& aFilePath)
{
	myCubeMap = aFilePath;
}