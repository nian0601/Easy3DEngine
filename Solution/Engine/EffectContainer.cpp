#include "stdafx.h"

#include "BaseEffect.h"
#include <d3dx11effect.h>
#include "Engine.h"
#include "Effect2D.h"
#include "Effect3D.h"
#include "EffectContainer.h"
#include "FileWatcher.h"
#include "Texture.h"
#include "TextureContainer.h"
#include "ParticleEffect.h"

namespace Easy3D
{
	EffectContainer* EffectContainer::myInstance = nullptr;

	void EffectContainer::Create()
	{
		myInstance = new EffectContainer();
	}

	void EffectContainer::Destroy()
	{
		delete myInstance;
	}

	EffectContainer* EffectContainer::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "EffectContainer: myInstance is nullptr, forgot to create?");
		return myInstance;
	}

	EffectContainer::~EffectContainer()
	{
		for (auto it = myBaseEffects.begin(); it != myBaseEffects.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}

		for (auto it = my2DEffects.begin(); it != my2DEffects.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}

		for (auto it = my3DEffects.begin(); it != my3DEffects.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}

		for (auto it = myParticleEffects.begin(); it != myParticleEffects.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	BaseEffect* EffectContainer::GetBaseEffect(const std::string& aFilePath)
	{
		auto it = myBaseEffects.find(aFilePath);

		if (it == myBaseEffects.end())
		{
			LoadBaseEffect(aFilePath);
		}

		return myBaseEffects[aFilePath];
	}

	Effect2D* EffectContainer::Get2DEffect(const std::string& aFilePath)
	{
		auto it = my2DEffects.find(aFilePath);

		if (it == my2DEffects.end())
		{
			Load2DEffect(aFilePath);
		}

		return my2DEffects[aFilePath];
	}

	Effect3D* EffectContainer::Get3DEffect(const std::string& aFilePath)
	{
		auto it = my3DEffects.find(aFilePath);

		if (it == my3DEffects.end())
		{
			Load3DEffect(aFilePath);
		}

		return my3DEffects[aFilePath];
	}

	ParticleEffect* EffectContainer::GetParticleEffect(const std::string& aFilePath)
	{
		auto it = myParticleEffects.find(aFilePath);

		if (it == myParticleEffects.end())
		{
			LoadParticleEffect(aFilePath);
		}

		return myParticleEffects[aFilePath];
	}


	void EffectContainer::LoadBaseEffect(const std::string& aFilePath)
	{
		BaseEffect* newEffect = new BaseEffect();

		if (newEffect->Init(aFilePath) == false)
		{
			std::stringstream ss;
			ss << "Failed to Init Effect: " << aFilePath.c_str() << std::endl;
			DL_MESSAGE_BOX(ss.str().c_str(), "EffectContainer::LoadBaseEffect", MB_ICONWARNING);
			return;
		}


		DL_ASSERT_EXP(newEffect != nullptr, "newEffect is nullpter in LoadEffect, HOW?");

		myBaseEffects[aFilePath] = newEffect;

		WATCH_FILE(aFilePath, EffectContainer::ReloadBaseEffect);
	}

	void EffectContainer::ReloadBaseEffect(const std::string& aFilePath)
	{
		if (myBaseEffects.find(aFilePath) == myBaseEffects.end())
		{
			return;
		}

		myBaseEffects[aFilePath]->Init(aFilePath);
	}

	void EffectContainer::Load2DEffect(const std::string& aFilePath)
	{
		Effect2D* newEffect = new Effect2D();

		if (newEffect->Init(aFilePath) == false)
		{
			std::stringstream ss;
			ss << "Failed to Init Effect: " << aFilePath.c_str() << std::endl;
			DL_MESSAGE_BOX(ss.str().c_str(), "EffectContainer::Load2DEffect", MB_ICONWARNING);
			return;
		}


		DL_ASSERT_EXP(newEffect != nullptr, "newEffect is nullpter in LoadEffect, HOW?");

		my2DEffects[aFilePath] = newEffect;

		WATCH_FILE(aFilePath, EffectContainer::Reload2DEffect);
	}

	void EffectContainer::Reload2DEffect(const std::string& aFilePath)
	{
		if (my2DEffects.find(aFilePath) == my2DEffects.end())
		{
			return;
		}

		my2DEffects[aFilePath]->Init(aFilePath);
	}


	void EffectContainer::Load3DEffect(const std::string& aFilePath)
	{
		Effect3D* newEffect = new Effect3D();

		if (newEffect->Init(aFilePath) == false)
		{
			std::stringstream ss;
			ss << "Failed to Init Effect: " << aFilePath.c_str() << std::endl;
			DL_MESSAGE_BOX(ss.str().c_str(), "EffectContainer::Load3DEffect", MB_ICONWARNING);
			return;
		}

		Texture* tex = TextureContainer::GetInstance()->GetTexture(myCubeMap);
		ID3DX11EffectShaderResourceVariable* shaderVar = newEffect->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

		if (shaderVar->IsValid())
		{
			shaderVar->SetResource(tex->GetShaderView());
		}


		DL_ASSERT_EXP(newEffect != nullptr, "newEffect is nullpter in LoadEffect, HOW?");

		my3DEffects[aFilePath] = newEffect;

		WATCH_FILE(aFilePath, EffectContainer::Reload3DEffect);
	}

	void EffectContainer::Reload3DEffect(const std::string& aFilePath)
	{
		if (my3DEffects.find(aFilePath) == my3DEffects.end())
		{
			return;
		}

		my3DEffects[aFilePath]->Init(aFilePath);


		Texture* tex = TextureContainer::GetInstance()->GetTexture(myCubeMap);
		ID3DX11EffectShaderResourceVariable* shaderVar = my3DEffects[aFilePath]->GetEffect()->GetVariableByName("CubeMap")->AsShaderResource();

		if (shaderVar->IsValid())
		{
			shaderVar->SetResource(tex->GetShaderView());
		}
	}


	void EffectContainer::LoadParticleEffect(const std::string& aFilePath)
	{
		ParticleEffect* newEffect = new ParticleEffect();

		if (newEffect->Init(aFilePath) == false)
		{
			std::stringstream ss;
			ss << "Failed to Init Effect: " << aFilePath.c_str() << std::endl;
			DL_MESSAGE_BOX(ss.str().c_str(), "EffectContainer::Load3DEffect", MB_ICONWARNING);
			return;
		}


		DL_ASSERT_EXP(newEffect != nullptr, "newEffect is nullpter in LoadEffect, HOW?");

		myParticleEffects[aFilePath] = newEffect;

		WATCH_FILE(aFilePath, EffectContainer::ReloadParticleEffect);
	}

	void EffectContainer::ReloadParticleEffect(const std::string& aFilePath)
	{
		if (myParticleEffects.find(aFilePath) == myParticleEffects.end())
		{
			return;
		}

		myParticleEffects[aFilePath]->Init(aFilePath);
	}

	void EffectContainer::SetCubeMap(const std::string& aFilePath)
	{
		myCubeMap = aFilePath;
	}
}