#include "stdafx.h"

#include <d3dx11effect.h>
#include "Engine.h"
#include "Effect.h"
#include "EffectContainer.h"
#include "FileWatcher.h"
#include "Texture.h"
#include "TextureContainer.h"

Easy3D::Effect* Easy3D::EffectContainer::GetEffect(const std::string& aFilePath)
{
	auto it = myEffects.find(aFilePath);

	if (it == myEffects.end())
	{
		LoadEffect(aFilePath);
	}

	return myEffects[aFilePath];
}

void Easy3D::EffectContainer::LoadEffect(const std::string& aFilePath)
{
	Effect* newEffect = new Effect();
	
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

	myEffects[aFilePath] = newEffect;

	WATCH_FILE(aFilePath, Easy3D::EffectContainer::ReloadEffect);
}

void Easy3D::EffectContainer::ReloadEffect(const std::string& aFilePath)
{
	if (myEffects.find(aFilePath) == myEffects.end())
	{
		return;
	}

	myEffects[aFilePath]->Init(aFilePath);


	Texture* tex = Engine::GetInstance()->GetTextureContainer()->GetTexture(myCubeMap);
	ID3DX11EffectShaderResourceVariable* shaderVar = myEffects[aFilePath]->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

	if (shaderVar->IsValid())
	{
		shaderVar->SetResource(tex->GetShaderView());
	}
}

void Easy3D::EffectContainer::Update(const float aDeltaTime)
{
	for (auto it = myEffects.begin(); it != myEffects.end(); ++it)
	{
		it->second->UpdateTime(aDeltaTime);
	}
}

void Easy3D::EffectContainer::SetCubeMap(const std::string& aFilePath)
{
	myCubeMap = aFilePath;
}